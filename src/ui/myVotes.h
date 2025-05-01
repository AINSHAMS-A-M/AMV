#pragma once

#include <QWidget>
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include "_structs.hpp"


class MyVotesPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyVotesPage(QWidget *parent = nullptr);

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

public:
    void show_cards();

private:
    // UI setup methods
    void createSidebar();
    void createVotesListPage();
    void createPollViewPage();

    // Action methods
    void onRemoveVoteClicked(const size_t& identifier);
    void onViewVotePollClicked(const size_t& identifier, RetrievePollDTO &selectedPoll);

    // Main UI components
    QWidget *sidebar;
    QWidget *content;
    QStackedWidget *stackedWidget;
    QWidget *votesListPage;
    QWidget *pollViewPage;

    // Layout for card display
    QWidget *scrollContent;
    QVBoxLayout *scrollLayout;

    // Layout for poll view
    QVBoxLayout *pollViewLayout;

    // Colors
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
};
