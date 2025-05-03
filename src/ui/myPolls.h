#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include "_structs.hpp"
#include "sidebar.h"

class MyPollsPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyPollsPage(QWidget *parent = nullptr);
    void populatePollList();
    void onBackToListClicked();
    QWidget *pollListView;
    QStackedWidget  *contentStack;

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private slots:
    void onViewPollClicked(size_t pollId);
    void onDeletePollClicked(size_t pollId);

private:
    SidebarWidget *sidebar;
    QWidget         *content;
    QWidget         *pollDetailsView;   
    QScrollArea     *pollListScrollArea;
    QWidget         *pollListContainerWidget;
    QVBoxLayout     *pollListLayout;
    QVBoxLayout     *pollDetailsLayout;
    QPushButton     *backButton;
    QLabel          *pollDetailsTitleLabel;
    QTableWidget    *optionsTable;
    void setupPollListView();
    void setupPollDetailsView();
    void displayPollDetails(const RetrievePollResultAdmin& poll);
};
