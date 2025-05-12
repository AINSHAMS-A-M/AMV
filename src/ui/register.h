#pragma once

#include <QString>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class RegisterPage : public QWidget
{
	Q_OBJECT

public:
	explicit RegisterPage(QWidget* parent = nullptr);
	~RegisterPage() override = default;
	void onRegisterClicked();
	QLineEdit* realNameEdit;
	QLineEdit* usernameEdit;
	QLineEdit* passwordEdit;
	QLineEdit* confirmEdit;
	QLineEdit* addressEdit;
	QLineEdit* emailEdit;
	QLineEdit* phoneNumberEdit;

signals:
	void loginLinkActivated();
	void registrationSuccessful();


private:
	QWidget* leftPanel;
	QWidget* rightPanel;
	QPushButton* registerBtn;
	QLabel* loginLabel;
};
