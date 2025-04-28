#pragma once

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage() override = default;
    QString userName() const;
    QString password() const;

signals:
    void loginClicked();
    void registerLinkActivated(const QString &link);

private:
    QWidget    *leftPanel;
    QWidget    *rightPanel;
    QLineEdit  *userEdit;
    QLineEdit  *passEdit;
    QPushButton *loginBtn;
    QLabel     *registerLabel;
};

