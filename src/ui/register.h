#pragma once

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage() override = default;
    void onRegisterClicked();
    QLineEdit   *realNameEdit;
    QLineEdit   *usernameEdit;
    QLineEdit   *passwordEdit;
    QLineEdit   *confirmEdit;

signals:
    void loginLinkActivated();
    void registrationSuccessful();


private:
    QWidget     *leftPanel;
    QWidget     *rightPanel;
    QPushButton *registerBtn;
    QLabel      *loginLabel;
};
