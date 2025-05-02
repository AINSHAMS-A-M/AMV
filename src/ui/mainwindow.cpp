#include "mainwindow.h"
#include "nav.h"
#include "QLineEdit"
#include "login.h"
#include "QMenuBar"
#include "QStatusBar"
#include "register.h"
#include "help.h"
#include "createPoll.h"
#include "myPolls.h"
#include "myVotes.h"
#include "profile.h"
#include <QMessageBox>
#include "vote.h"
#include "db.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    stackedWidget (new QStackedWidget(this))
{
    // Hide the grey bars in the top and bottom of the window
    menuBar()->hide();
    statusBar()->hide();

    setCentralWidget(stackedWidget);

    if (!loginPage)
    {
        loginPage     = new LoginPage(this);
        stackedWidget->addWidget(loginPage);     // index 0
    }

    if (!registerPage)
    {
        registerPage  = new RegisterPage(this);
        stackedWidget->addWidget(registerPage);  // index 1
    }

    stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LoginPage::loginSuccessful,
            this,      &MainWindow::onLoginClicked);

    connect(loginPage, &LoginPage::registerLinkActivated,
            this,      &MainWindow::onRegisterLink);

    connect(registerPage, &RegisterPage::registrationSuccessful,
            this,          &MainWindow::onLoginLink);

    connect(registerPage, &RegisterPage::loginLinkActivated,
            this,          &MainWindow::onLoginLink);
}


void MainWindow::onLoginClicked()
{

    init_app();

    connect(&NavigationManager::instance(),
            &NavigationManager::navigateTo,
            this, &MainWindow::handleNavigation);

    stackedWidget->setCurrentWidget(helpPage);
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
    myPollsPage->populatePollList();
}

void MainWindow::onMyVotesClicked()
{
    stackedWidget->setCurrentWidget(myVotesPage);
    myVotesPage->show_cards();
}

void MainWindow::onVoteClicked()
{
    stackedWidget->setCurrentWidget(votePage);
    votePage->voterIdInput->clear();
    votePage->voterIdInput->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Ask the user if they want to quit
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Exit",
        "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Allow the window to close
        save_data();
        event->accept();
    } else {
        // Prevent the window from closing
        event->ignore();
    }
}

void MainWindow::init_app()
{
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

void MainWindow::handleNavigation(NavigationManager::Page page) {
    switch (page) {
    case NavigationManager::Login:    onLoginLink(); break;
    case NavigationManager::Register: onRegisterLink(); break;
    case NavigationManager::Help:     onHelpClicked(); break;
    case NavigationManager::Vote:     onVoteClicked(); break;
    case NavigationManager::CreatePoll: onCreatePollClicked(); break;
    case NavigationManager::MyPolls:  onMyPollsClicked(); break;
    case NavigationManager::MyVotes:  onMyVotesClicked(); break;
    case NavigationManager::Profile:  onProfileClicked(); break;
    }
}

