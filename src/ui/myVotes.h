#ifndef MYVOTES_H
#define MYVOTES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStackedWidget>
#include <QString>
#include "_structs.hpp"
#include "sidebar.h"

class MyVotesPage : public QWidget {
    Q_OBJECT

public:
    explicit MyVotesPage(QWidget *parent = nullptr);
    QWidget *votesListPage;
    void show_cards();
    QStackedWidget *stackedWidget;

signals:
    // Sidebar actions
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private:
    // Sidebar and main content
   SidebarWidget *sidebar;
    QWidget *content;

    // Stacked pages

    QWidget *pollViewPage;
    QVBoxLayout* pollViewLayout;

    // Vote list scroll
    QWidget *scrollContent;
    QVBoxLayout *scrollLayout;

    // Color palette
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

    // Initialization helpers
    void createSidebar();
    void createVotesListPage();
    void createPollViewPage();

    // Action handlers
    void onRemoveVoteClicked(const size_t &identifier);
    void onViewVotePollClicked(const size_t &identifier, RetrievePollDTO &selectedPoll);
};

#endif // MYVOTES_H
