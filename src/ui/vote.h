#ifndef VOTE_H
#define VOTE_H

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
    QLabel *pollTitleLabel;
    QLabel *pollDescLabel;
    QWidget *optionsContainer;
    QVBoxLayout *optionsLayout;
    QButtonGroup *optionsGroup;
    size_t pollId;
    void loadPollOptions();
};

#endif
