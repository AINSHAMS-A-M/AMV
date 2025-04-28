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

signals:
    void registerClicked();
    void loginLinkActivated(const QString &link);

private:
    QWidget     *leftPanel;
    QWidget     *rightPanel;
    QLineEdit   *realNameEdit;
    QLineEdit   *usernameEdit;
    QLineEdit   *passwordEdit;
    QLineEdit   *confirmEdit;
    QPushButton *registerBtn;
    QLabel      *loginLabel;
};
