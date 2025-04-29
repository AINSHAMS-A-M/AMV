#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Keep forward declarations as they were
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
    ~MainWindow() override;

protected:
    // Override the close event handler
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();
    void on_register_linkActivated(const QString &link);
    void on_login_linkActivated(const QString &link);
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private:
    Ui::MainWindow *ui;
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

#endif // MAINWINDOW_H
