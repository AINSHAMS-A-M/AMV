#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include "_structs.hpp"
#include "db.hpp"
#include "sidebar.h"
#include "utils.h"

class MyPollsPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyPollsPage(QWidget *parent = nullptr);

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private slots:
    // Poll list view slots
    void populatePollList(); // Fetches and displays user's polls
    void onViewPollClicked(size_t pollId);

    // Poll details view slots
    void onBackToListClicked(); // Handles click on "Back" button

private:
    // UI Components
    SidebarWidget *sidebar;
    QWidget         *content;
    QStackedWidget  *contentStack;

    // Pages within the contentStack
    QWidget         *pollListView;      // Page showing list of polls as cards
    QWidget         *pollDetailsView;   // Page showing details of a selected poll

    // Widgets within pollListView
    QScrollArea     *pollListScrollArea;
    QWidget         *pollListContainerWidget; // Widget inside scroll area
    QVBoxLayout     *pollListLayout;          // Layout for poll cards

    // Widgets within pollDetailsView
    QVBoxLayout     *pollDetailsLayout;
    QPushButton     *backButton;
    QLabel          *pollDetailsTitleLabel;
    QTableWidget    *optionsTable; // To display options and vote counts

    // Helper function for UI setup
    void setupPollListView();
    void setupPollDetailsView();
    void displayPollDetails(const RetrievePollResultAdmin& poll); // Populates the details view
};
