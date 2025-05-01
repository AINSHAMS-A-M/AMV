#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>


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

protected:
    // Override the close event handler
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
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

    // Maintain original pointer names
    LoginPage       *loginPage;
    RegisterPage    *registerPage;
    VotePage        *votePage;
    MyVotesPage     *myVotesPage;
    MyPollsPage     *myPollsPage;
    ProfileEditPage *profileEditPage;
    CreatePollPage  *createPollPage;
    HelpPage        *helpPage;
};

