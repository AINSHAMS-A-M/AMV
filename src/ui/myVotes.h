#ifndef MYVOTES_H
#define MYVOTES_H

#include "_db_structs.hpp"
#include "_structs.hpp"
#include "sidebar.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStackedWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class MyVotesPage : public QWidget
{
	Q_OBJECT

public:
	explicit MyVotesPage(QWidget* parent = nullptr);
	QWidget* votesListPage;
	void show_cards();
	QStackedWidget* stackedWidget;
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
	QWidget* pollViewPage;
	QVBoxLayout* pollViewLayout;
	QWidget* scrollContent;
	QVBoxLayout* scrollLayout;
	QString sidebarColor;
	QString bgColor;
	QString primaryColor;
	QString primaryHover;
	QString textColor;
	QString accentColor;
	QString dangerColor;
	QString dangerHoverBg;
	QString dangerHoverFg;
	QString cardBgColor;
	void createVotesListPage();
	void createPollViewPage();
	void onRemoveVoteClicked(const size_t& identifier);
	void onViewVotePollClicked(const size_t& id, RetrievePollResultAdmin& selectedPoll);
};

#endif
