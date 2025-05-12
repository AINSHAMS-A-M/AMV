#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "QBoxLayout"
#include "qlabel.h"
#include <QPushButton>
#include <QStringList>
#include <QWidget>

class SidebarWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SidebarWidget(QWidget* parent, std::string page);
	void setupWelcomeLabel();
	QLabel* welcomeLabel;

private:
	void setupMenuItems(std::string page);

	void setupLogoutButton();
	QVBoxLayout* sbLayout;
};


#endif
