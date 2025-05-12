#include "createPoll.h"
#include "db.hpp"
#include "help.h"
#include "login.h"
#include "mainwindow.h"
#include "myPolls.h"
#include "myVotes.h"
#include "nav.h"
#include "profile.h"
#include <QApplication>
#include <QLineEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <register.h>
#include <sidebar.h>
#include <vote.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
{
    setupUI();
}

void MainWindow::setupUI()
{
    // Hide default menu bar and status bar
    menuBar()->hide();
    statusBar()->hide();

    setCentralWidget(stackedWidget);

    // Initialize login page if not already created
    if (!loginPage)
    {
        loginPage = new LoginPage(this);
        stackedWidget->addWidget(loginPage); // index 0
    }

    // Initialize register page if not already created
    if (!registerPage)
    {
        registerPage = new RegisterPage(this);
        stackedWidget->addWidget(registerPage); // index 1
    }

    // Start with login page visible
    stackedWidget->setCurrentWidget(loginPage);

    // Connect signals to handlers
    connect(loginPage, &LoginPage::loginSuccessful,
            this, &MainWindow::onLoginClicked);

    connect(loginPage, &LoginPage::registerLinkActivated,
            this, &MainWindow::onRegisterLink);

    connect(registerPage, &RegisterPage::registrationSuccessful,
            this, &MainWindow::onLoginLink);

    connect(registerPage, &RegisterPage::loginLinkActivated,
            this, &MainWindow::onLoginLink);
}

void MainWindow::onLoginClicked()
{
    // Initialize application pages after successful login
    init_app();

    // Connect navigation manager
    connect(&NavigationManager::instance(),
            &NavigationManager::navigateTo,
            this, &MainWindow::handleNavigation);

    connect(profileEditPage, &ProfileEditPage::changeWelcomeLabel,
            this, &MainWindow::changeLabelText);

    // Show help page by default
    stackedWidget->setCurrentWidget(helpPage);
}

void MainWindow::onLogoutClicked()
{
    // Delete all dynamically created pages to reset state
    if (helpPage)
    {
        delete helpPage;
        helpPage = nullptr;
    }
    if (votePage)
    {
        delete votePage;
        votePage = nullptr;
    }
    if (createPollPage)
    {
        delete createPollPage;
        createPollPage = nullptr;
    }
    if (myPollsPage)
    {
        delete myPollsPage;
        myPollsPage = nullptr;
    }
    if (myVotesPage)
    {
        delete myVotesPage;
        myVotesPage = nullptr;
    }
    if (profileEditPage)
    {
        delete profileEditPage;
        profileEditPage = nullptr;
    }

    // Remove all widgets from stacked widget
    while (stackedWidget->count() > 0)
    {
        stackedWidget->removeWidget(stackedWidget->widget(0));
    }

    // Delete login and register pages to force recreation
    if (loginPage)
    {
        delete loginPage;
        loginPage = nullptr;
    }
    if (registerPage)
    {
        delete registerPage;
        registerPage = nullptr;
    }

    // Re-setup UI with fresh login/register pages
    setupUI();
    loginPage->userEdit->setFocus();
}

void MainWindow::onRegisterLink()
{
    stackedWidget->setCurrentWidget(registerPage);
    registerPage->realNameEdit->setFocus();
}

void MainWindow::onLoginLink()
{
    stackedWidget->setCurrentWidget(loginPage);
    loginPage->userEdit->setFocus();
}

void MainWindow::onProfileClicked()
{
    profileEditPage->onEditProfileClicked();
    stackedWidget->setCurrentWidget(profileEditPage);
}

void MainWindow::onHelpClicked()
{
    stackedWidget->setCurrentWidget(helpPage);
}

void MainWindow::onCreatePollClicked()
{
    createPollPage->resetOptions();
    createPollPage->pollNameEdit->clear();
    createPollPage->pollDescEdit->clear();
    createPollPage->voterIdEdit->clear();
    createPollPage->pollNameEdit->setFocus();
    stackedWidget->setCurrentWidget(createPollPage);
}

void MainWindow::onMyPollsClicked()
{
    stackedWidget->setCurrentWidget(myPollsPage);
    myPollsPage->contentStack->setCurrentWidget(myPollsPage->pollListView);
    myPollsPage->populatePollList();
}

void MainWindow::onMyVotesClicked()
{
    stackedWidget->setCurrentWidget(myVotesPage);
    myVotesPage->stackedWidget->setCurrentWidget(myVotesPage->votesListPage);
    myVotesPage->show_cards();
}

void MainWindow::onVoteClicked()
{
    stackedWidget->setCurrentWidget(votePage);
    votePage->showVoterIdPage();
    votePage->voterIdInput->clear();
    votePage->voterIdInput->setFocus();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Ask user for confirmation before closing
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Exit",
        "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
    {
        save_data();  // Save data before exiting
        event->accept();  // Allow window to close
    }
    else
    {
        event->ignore();  // Cancel close action
    }
}

void MainWindow::init_app()
{
    // Lazily initialize app pages only once
    if (!helpPage)
    {
        helpPage = new HelpPage(this);
        stackedWidget->addWidget(helpPage); // index 2
    }
    if (!votePage)
    {
        votePage = new VotePage(this);
        stackedWidget->addWidget(votePage); // index 3
    }
    if (!createPollPage)
    {
        createPollPage = new CreatePollPage(this);
        stackedWidget->addWidget(createPollPage); // index 4
    }
    if (!myPollsPage)
    {
        myPollsPage = new MyPollsPage(this);
        stackedWidget->addWidget(myPollsPage); // index 5
    }
    if (!myVotesPage)
    {
        myVotesPage = new MyVotesPage(this);
        stackedWidget->addWidget(myVotesPage); // index 6
    }
    if (!profileEditPage)
    {
        profileEditPage = new ProfileEditPage(this);
        stackedWidget->addWidget(profileEditPage); // index 7
    }
}

void MainWindow::changeLabelText()
{
    // Update welcome label across all sidebars
    QString name = QString::fromStdString(activeUser.name);

    helpPage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
    createPollPage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
    myPollsPage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
    myVotesPage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
    profileEditPage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
    votePage->sidebar->welcomeLabel->setText("Welcome\n" + name + "!");
}

void MainWindow::handleNavigation(NavigationManager::Page page)
{
    // Handle navigation requests from NavigationManager
    switch (page)
    {
    case NavigationManager::Login:       onLoginLink(); break;
    case NavigationManager::Register:    onRegisterLink(); break;
    case NavigationManager::Help:        onHelpClicked(); break;
    case NavigationManager::Vote:        onVoteClicked(); break;
    case NavigationManager::CreatePoll:  onCreatePollClicked(); break;
    case NavigationManager::MyPolls:     onMyPollsClicked(); break;
    case NavigationManager::MyVotes:     onMyVotesClicked(); break;
    case NavigationManager::Profile:     onProfileClicked(); break;
    case NavigationManager::Logout:      onLogoutClicked(); break;
    }
}
