#include "mainwindow.h"
#include "qlineedit"
#include "ui_mainwindow.h"
#include "utils.h"
#include "login.h"
#include "register.h"
#include "help.h"
#include "createPoll.h"
#include "myPolls.h"
#include "myVotes.h"
#include "profile.h"
#include <QMessageBox>
#include "vote.h"
#include <iostream>
#include "services.hpp"
#include "data_structures.hpp"
#include "db.hpp"
#include "_structs.hpp"
#include "_hash.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuBar()->hide();
    statusBar()->hide();

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    loginPage     = new LoginPage(this);
    registerPage  = new RegisterPage(this);

    stackedWidget->addWidget(loginPage);     // index 0
    stackedWidget->addWidget(registerPage);  // index 1

    stackedWidget->setCurrentWidget(loginPage);

    connect(loginPage, &LoginPage::loginClicked,
            this,      &MainWindow::on_loginBtn_clicked);

    connect(loginPage, &LoginPage::registerLinkActivated,
            this,      &MainWindow::on_register_linkActivated);

    connect(registerPage, &RegisterPage::registerClicked,
            this,          &MainWindow::on_registerBtn_clicked);

    connect(registerPage, &RegisterPage::loginLinkActivated,
            this,          &MainWindow::on_login_linkActivated);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    auto username = (loginPage->findChild<QLineEdit*>("userEdit"))->text().toStdString();
    auto password = (loginPage->findChild<QLineEdit*>("passEdit"))->text().toStdString();

    //std::string response =  log_in(username,password);
    std::string response = "true";
    if (response == "true")
    {
        activeUser = get_id_by_user(username);
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
    else
    {
        QMessageBox::warning(this,"Warning",QString::fromStdString(response));
    }
}

void MainWindow::on_registerBtn_clicked()
{
    auto username = (registerPage->findChild<QLineEdit*>("usernameEdit"))->text().toStdString();
    auto password = (registerPage->findChild<QLineEdit*>("passwordEdit"))->text().toStdString();
    auto confirm = (registerPage->findChild<QLineEdit*>("confirmEdit"))->text().toStdString();
    auto realname = (registerPage->findChild<QLineEdit*>("realnameEdit"))->text().toStdString();

    if (username == "" || realname == "" || password == "" || confirm == "")
    {
        QMessageBox::warning(this,"Warning","Fields cannot be empty!");
    }
    else if (username.find('`') != std::string::npos|| realname.find('`') != std::string::npos)
    {
        QMessageBox::warning(this,"Warning","Invalid Character! don't type \"`\" ");
    }
    else if (password == confirm)
    {
        bool capital = 0, small = 0, number = 0, special = 0;
        for (auto ch : password)
        {
            if (ch >= 'a' && ch <= 'z') small = 1;
            else if (ch >= 'A' && ch <= 'Z') capital = 1;
            else if (ch >= '0' && ch <= '9') number = 1;
            else special = 1;
        }

        if (capital && small && number && special && password.size() >= 8)
        {
            size_t new_id = users.size();

            CreateUser newUser =
                {
                    new_id,
                    realname,
                    hash_password(password,new_id),
                    username
                };
            auto response = create_user(newUser);
            if (response == "Em")
            {
                QMessageBox::warning(this,"Warning","Fields cannot be empty!");
            }
            else if (response == "Already Exist")
            {
                QMessageBox::warning(this,"Warning","User already exists! Please login");
            }
            else if (response == "Success")
            {
                stackedWidget->setCurrentWidget(loginPage);
            }
        }
        else
        {
            QMessageBox::warning(this,"Warning","Invalid password, Please note that the "
                                                  "password must contain at least: a capital letter,"
                                                  "a small letter, a number, and a special character"
                                                  "(!,@,#,$,%,^,&,/,\\,,) and must at least be 8 characters!");
        }


    }
    else
    {
        QMessageBox::warning(this,"Warning","Passwords don't match!!");
    }
}

void MainWindow::on_register_linkActivated(const QString &)
{
    stackedWidget->setCurrentWidget(registerPage);
}

void MainWindow::on_login_linkActivated(const QString &)
{
    stackedWidget->setCurrentWidget(loginPage);
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

