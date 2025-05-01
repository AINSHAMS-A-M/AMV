#include "mainwindow.h"
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

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    loginPage     = new LoginPage(this);
    registerPage  = new RegisterPage(this);

    stackedWidget->addWidget(loginPage);     // index 0
    stackedWidget->addWidget(registerPage);  // index 1

    stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LoginPage::loginSuccessful,
            this,      &MainWindow::onLoginClicked);

    connect(loginPage, &LoginPage::registerLinkActivated,
            this,      &MainWindow::onRegisterLink);

    connect(registerPage, &RegisterPage::registrationSuccessful,
            this,          &MainWindow::onRegisterClicked);

    connect(registerPage, &RegisterPage::loginLinkActivated,
            this,          &MainWindow::onLoginLink);
}

void MainWindow::onRegisterClicked()
{
    stackedWidget->setCurrentWidget(loginPage);
    QLineEdit* userEdit = loginPage->findChild<QLineEdit*>("userEdit");
    if (userEdit) {
        userEdit->setFocus();
        userEdit->selectAll(); // Optional: Select all text for easy editing
    }
}
void MainWindow::onLoginClicked()
{
    helpPage = new HelpPage(this);
    votePage = new VotePage(this);
    createPollPage = new CreatePollPage(this);
    myPollsPage = new MyPollsPage(this);
    myVotesPage = new MyVotesPage(this);
    profileEditPage = new ProfileEditPage(this);
    stackedWidget->addWidget(helpPage); // index 2
    stackedWidget->addWidget(votePage); // index 3
    stackedWidget->addWidget(createPollPage); // index 4
    stackedWidget->addWidget(myPollsPage); // index 5
    stackedWidget->addWidget(myVotesPage); // index 6
    stackedWidget->addWidget(profileEditPage); // index 7

    connect(helpPage,       &HelpPage::onHelpClicked,       this, &MainWindow::onHelpClicked);
    connect(votePage,       &VotePage::onHelpClicked,       this, &MainWindow::onHelpClicked);
    connect(myVotesPage,    &MyVotesPage::onHelpClicked,    this, &MainWindow::onHelpClicked);
    connect(createPollPage, &CreatePollPage::onHelpClicked, this, &MainWindow::onHelpClicked);
    connect(myPollsPage,    &MyPollsPage::onHelpClicked,    this, &MainWindow::onHelpClicked);
    connect(profileEditPage,&ProfileEditPage::onHelpClicked,this, &MainWindow::onHelpClicked);

    // Connecting onVoteClicked signal from all relevant pages
    connect(helpPage,       &HelpPage::onVoteClicked,       this, &MainWindow::onVoteClicked);
    connect(votePage,       &VotePage::onVoteClicked,       this, &MainWindow::onVoteClicked);
    connect(myVotesPage,    &MyVotesPage::onVoteClicked,    this, &MainWindow::onVoteClicked);
    connect(createPollPage, &CreatePollPage::onVoteClicked, this, &MainWindow::onVoteClicked);
    connect(myPollsPage,    &MyPollsPage::onVoteClicked,    this, &MainWindow::onVoteClicked);
    connect(profileEditPage,&ProfileEditPage::onVoteClicked,this, &MainWindow::onVoteClicked);

    // Connecting onMyVotesClicked signal from all relevant pages
    connect(helpPage,       &HelpPage::onMyVotesClicked,       this, &MainWindow::onMyVotesClicked);
    connect(votePage,       &VotePage::onMyVotesClicked,       this, &MainWindow::onMyVotesClicked);
    connect(myVotesPage,    &MyVotesPage::onMyVotesClicked,    this, &MainWindow::onMyVotesClicked);
    connect(createPollPage, &CreatePollPage::onMyVotesClicked, this, &MainWindow::onMyVotesClicked);
    connect(myPollsPage,    &MyPollsPage::onMyVotesClicked,    this, &MainWindow::onMyVotesClicked);
    connect(profileEditPage,&ProfileEditPage::onMyVotesClicked,this, &MainWindow::onMyVotesClicked);

    // Connecting onCreatePollClicked signal from all relevant pages
    connect(helpPage,       &HelpPage::onCreatePollClicked,       this, &MainWindow::onCreatePollClicked);
    connect(votePage,       &VotePage::onCreatePollClicked,       this, &MainWindow::onCreatePollClicked);
    connect(myVotesPage,    &MyVotesPage::onCreatePollClicked,    this, &MainWindow::onCreatePollClicked);
    connect(createPollPage, &CreatePollPage::onCreatePollClicked, this, &MainWindow::onCreatePollClicked);
    connect(myPollsPage,    &MyPollsPage::onCreatePollClicked,    this, &MainWindow::onCreatePollClicked);
    connect(profileEditPage,&ProfileEditPage::onCreatePollClicked,this, &MainWindow::onCreatePollClicked);

    // Connecting onMyPollsClicked signal from all relevant pages
    connect(helpPage,       &HelpPage::onMyPollsClicked,       this, &MainWindow::onMyPollsClicked);
    connect(votePage,       &VotePage::onMyPollsClicked,       this, &MainWindow::onMyPollsClicked);
    connect(myVotesPage,    &MyVotesPage::onMyPollsClicked,    this, &MainWindow::onMyPollsClicked);
    connect(createPollPage, &CreatePollPage::onMyPollsClicked, this, &MainWindow::onMyPollsClicked);
    connect(myPollsPage,    &MyPollsPage::onMyPollsClicked,    this, &MainWindow::onMyPollsClicked);
    connect(profileEditPage,&ProfileEditPage::onMyPollsClicked,this, &MainWindow::onMyPollsClicked);

    // Connecting onProfileClicked signal from all relevant pages
    connect(helpPage,       &HelpPage::onProfileClicked,       this, &MainWindow::onProfileClicked);
    connect(votePage,       &VotePage::onProfileClicked,       this, &MainWindow::onProfileClicked);
    connect(myVotesPage,    &MyVotesPage::onProfileClicked,    this, &MainWindow::onProfileClicked);
    connect(createPollPage, &CreatePollPage::onProfileClicked, this, &MainWindow::onProfileClicked);
    connect(myPollsPage,    &MyPollsPage::onProfileClicked,    this, &MainWindow::onProfileClicked);
    connect(profileEditPage,&ProfileEditPage::onProfileClicked,this, &MainWindow::onProfileClicked);

    stackedWidget->setCurrentWidget(helpPage);
}



void MainWindow::onRegisterLink()
{
    stackedWidget->setCurrentWidget(registerPage);
    QLineEdit* realEdit = registerPage->findChild<QLineEdit*>("realnameEdit");
    if (realEdit) {
        realEdit->setFocus();
    }
}

void MainWindow::onLoginLink()
{
    stackedWidget->setCurrentWidget(loginPage);
    QLineEdit* userEdit = loginPage->findChild<QLineEdit*>("userEdit");
    if (userEdit) {
        userEdit->setFocus();
    }
}

void MainWindow::onProfileClicked()
{
    stackedWidget->setCurrentWidget(profileEditPage);
}

void MainWindow::onHelpClicked()
{
    stackedWidget->setCurrentWidget(helpPage);
}

void MainWindow::onCreatePollClicked()
{
    stackedWidget->setCurrentWidget(createPollPage);
}

void MainWindow::onMyPollsClicked()
{
    stackedWidget->setCurrentWidget(myPollsPage);
}

void MainWindow::onMyVotesClicked()
{
    stackedWidget->setCurrentWidget(myVotesPage);
    myVotesPage->show_cards();
}

void MainWindow::onVoteClicked()
{
    stackedWidget->setCurrentWidget(votePage);
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

