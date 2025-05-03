#pragma once
#include "sidebar.h"
#include <QWidget>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QLabel;
class QTextEdit;
class QWidget;
class QPushButton;
class QLineEdit;
class QStackedWidget;
QT_END_NAMESPACE

class ProfileEditPage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfileEditPage(QWidget *parent = nullptr);
    ~ProfileEditPage() override = default;
    QLineEdit *usernameField;
    QLineEdit *realNameField;
    QLineEdit *oldPasswordField;
    QLineEdit *newPasswordField;
    QLineEdit *confirmPasswordField;
    SidebarWidget *sidebar;

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();
    void changeWelcomeLabel();

public slots:
    void onSaveProfileClicked();
    void onUpdatePasswordClicked();
    void onEditProfileClicked();

private:
    QWidget *content;
    QStackedWidget *stackedWidget;
    QList<QPushButton*> sidebarButtons;
    QPushButton *profileInfoBtn;
    QPushButton *changePasswordTabBtn;
    QString bgColor;
    QString cardColor;
    QString borderColor;
    QString primaryBtnColor;
    QString primaryBtnHoverColor;
    QString primaryBtnPressedColor;
    QString labelColor;
    QString titleColor;
};
