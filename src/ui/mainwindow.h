#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>
#include "nav.h"


class LoginPage;
class RegisterPage;
class VotePage;
class MyVotesPage;
class MyPollsPage;
class ProfileEditPage;
class CreatePollPage;
class HelpPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
    void init_app();
    void handleNavigation(NavigationManager::Page page);
    void changeLabelText();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setupUI();
    void onLoginClicked();
    void onLogoutClicked();
    void onRegisterLink();
    void onLoginLink();
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private:
    QStackedWidget *stackedWidget;
    LoginPage       *loginPage = nullptr;
    RegisterPage    *registerPage= nullptr;
    VotePage        *votePage= nullptr;
    MyVotesPage     *myVotesPage= nullptr;
    MyPollsPage     *myPollsPage= nullptr;
    ProfileEditPage *profileEditPage= nullptr;
    CreatePollPage  *createPollPage= nullptr;
    HelpPage        *helpPage= nullptr;
};

