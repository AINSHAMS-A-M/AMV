#ifndef VOTE_H
#define VOTE_H

#include "data_structures.hpp"
#include "sidebar.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>

class QStackedWidget;

class VotePage : public QWidget
{
    Q_OBJECT

public:
    explicit VotePage(QWidget *parent = nullptr);
    QLineEdit *voterIdInput;

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

private slots:
    void validateVoterCredentials();
    void submitVote();
    void showVoterIdPage();


private:
    SidebarWidget *sidebar;
    QWidget *content;
    QStackedWidget *stackedWidget;
    QWidget *voterIdPage;
    QWidget *pollOptionsPage;



    // Poll display
    QLabel *pollTitleLabel;
    QLabel *pollDescLabel;
    QWidget *optionsContainer;
    QVBoxLayout *optionsLayout;
    QButtonGroup *optionsGroup;
    size_t pollId;

    // Helper methods
    void loadPollOptions();
};

#endif // VOTE_H
