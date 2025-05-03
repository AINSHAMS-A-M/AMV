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
    SidebarWidget *sidebar;

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
    QWidget         *content;
    QWidget         *pollDetailsView;   
    QScrollArea     *pollListScrollArea;
    QWidget         *pollListContainerWidget;
    QVBoxLayout     *pollListLayout;
    QVBoxLayout     *pollDetailsLayout;
    QPushButton     *backButton;
    QLabel          *pollDetailsTitleLabel;
    QTableWidget    *optionsTable;
    const QString bgColor       = "#F5F6F8";
    const QString dangerColor   = "#DC3545";
    const QString dangerHoverBg = "#F8D7DA";
    const QString dangerHoverFg = "#721C24";
    void setupPollListView();
    void setupPollDetailsView();
    void displayPollDetails(const RetrievePollResultAdmin& poll);
};
