#pragma once
#include <QWidget> // Base class
#include <QStringList> // Needed for menu items

QT_BEGIN_NAMESPACE
class QLabel;
class QTextEdit;
class QWidget;
class QPushButton; // Include QPushButton forward declaration
class QLineEdit; // For input fields
class QStackedWidget; // For stacked content
QT_END_NAMESPACE

class ProfileEditPage : public QWidget
{
    Q_OBJECT
public:
    // Constructor declaration only
    explicit ProfileEditPage(QWidget *parent = nullptr);
    ~ProfileEditPage() override = default;

signals:
    // Signals to indicate which sidebar menu item was clicked
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

public slots:
    void onSaveProfileClicked();
    void onUpdatePasswordClicked();

private:
    QWidget *sidebar;
    QWidget *content;
    QStackedWidget *stackedWidget;
    QList<QPushButton*> sidebarButtons;

    // Profile form fields
    QLineEdit *usernameField;
    QLineEdit *realNameField;

    // Password form fields
    QLineEdit *oldPasswordField;
    QLineEdit *newPasswordField;
    QLineEdit *confirmPasswordField;

    // Tab navigation buttons
    QPushButton *profileInfoBtn;
    QPushButton *changePasswordTabBtn;
};
