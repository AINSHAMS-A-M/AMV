#pragma once

#include <QString>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class LoginPage : public QWidget
{
	Q_OBJECT

public:
	explicit LoginPage(QWidget* parent = nullptr);
	~LoginPage() override = default;
	void onLoginClicked();
	QLineEdit* userEdit;
	QLineEdit* passEdit;

signals:
	void loginSuccessful();
	void registerLinkActivated();

private:
	QWidget* leftPanel;
	QWidget* rightPanel;
	QPushButton* loginBtn;
	QLabel* registerLabel;
};

