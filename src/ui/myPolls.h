#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include "_structs.hpp"
#include "qtextedit.h"
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
    void setupPollDetailsView();

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
    void onEditDescriptionClicked(size_t pollId);
    void onCustomizeOptionsClicked(size_t pollId);
    void onSaveDescriptionClicked();
    void onSaveOptionsClicked();
    void onCancelEditDescriptionClicked();
    void onCancelCustomizeOptionsClicked();

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
    QWidget *editDescriptionView;
    QWidget *customizeOptionsView;
    QVBoxLayout *editDescriptionLayout;
    QLabel *editDescriptionTitleLabel;
    QLineEdit *descriptionTextEdit;
    QPushButton *saveDescriptionButton;
    QPushButton *cancelEditDescriptionButton;
    QVBoxLayout *customizeOptionsLayout;
    QLabel *customizeOptionsTitleLabel;
    QTableWidget *editableOptionsTable;
    QPushButton *addOptionButton;
    QPushButton *saveOptionsButton;
    QPushButton *cancelCustomizeOptionsButton;
    const QString bgColor       = "#F5F6F8";
    const QString dangerColor   = "#DC3545";
    const QString dangerHoverBg = "#F8D7DA";
    const QString dangerHoverFg = "#721C24";
    void setupPollListView();
    void displayPollDetails(const RetrievePollResultAdmin& poll);
    void setupEditDescriptionView();
    void setupCustomizeOptionsView();
    void displayEditDescription();
    void displayCustomizeOptions();

    size_t currentPollId_ = 0;
};
