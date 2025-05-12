#pragma once

#include "sidebar.h"
#include <QStringList>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QTextEdit;
class QWidget;
class QPushButton;
QT_END_NAMESPACE

class HelpPage : public QWidget
{
	Q_OBJECT

public:
	explicit HelpPage(QWidget* parent = nullptr);
	~HelpPage() override = default;
	SidebarWidget* sidebar;

signals:
	void onHelpClicked();
	void onVoteClicked();
	void onMyVotesClicked();
	void onCreatePollClicked();
	void onMyPollsClicked();
	void onProfileClicked();

private:

	QWidget* content;
	QLabel* titleLabel;
	QTextEdit* helpTextEdit;
	QList<QPushButton*> sidebarButtons;

};
