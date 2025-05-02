// myVotes.cpp
#include "myVotes.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollArea>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPushButton>
#include <QDatetime>
#include <QFrame>
#include <QMessageBox>
#include <QStackedWidget>
#include "db.hpp"
#include "_structs.hpp"
#include "mainwindow.h"
#include "qscrollbar.h"
#include "services.hpp"

MyVotesPage::MyVotesPage(QWidget *parent)
    : QWidget(parent),
    votesListPage(nullptr),
    stackedWidget(nullptr),
    sidebar(nullptr),
    content(nullptr),
    pollViewPage(nullptr)
{
    bgColor       = "#F5F6F8";
    primaryColor  = "#007BFF";
    primaryHover  = "#339CFF";
    textColor     = "#FFFFFF";
    accentColor   = "#28A745";
    dangerColor   = "#DC3545";
    dangerHoverBg = "#F8D7DA";
    dangerHoverFg = "#721C24";
    cardBgColor   = "#FFFFFF";
    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this,"My Votes");

    // --- Create Content Area ---
    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    auto *contentMainLayout = new QVBoxLayout(content);
    contentMainLayout->setContentsMargins(0, 0, 0, 0);
    contentMainLayout->setSpacing(0);

    // Create stacked widget to handle page switching
    stackedWidget = new QStackedWidget(content);
    stackedWidget->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    // Create votes list page
    createVotesListPage();

    // Create poll view page (initially empty)
    createPollViewPage();

    // Add both pages to the stacked widget
    stackedWidget->addWidget(votesListPage);
    stackedWidget->addWidget(pollViewPage);

    // Start with votes list
    stackedWidget->setCurrentWidget(votesListPage);

    contentMainLayout->addWidget(stackedWidget);

    // --- Final Layout Setup ---
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content, 1);
}

void MyVotesPage::createVotesListPage()
{
    votesListPage = new QWidget(stackedWidget);
    votesListPage->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    auto *listLayout = new QVBoxLayout(votesListPage);
    listLayout->setContentsMargins(0, 0, 0, 0);
    listLayout->setSpacing(0);

    // Header Section
    auto *headerWidget = new QWidget(votesListPage);
    headerWidget->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(40, 40, 40, 30);

    QLabel *pageTitle = new QLabel("My Votes", headerWidget);
    pageTitle->setStyleSheet("font-size: 32px; font-weight: bold; color: #2C3E50; background: transparent; border: none;");
    headerLayout->addWidget(pageTitle);

    QLabel *pageDescription = new QLabel("View and manage your voting history across all polls.", headerWidget);
    pageDescription->setStyleSheet("font-size: 17px; color: #5D6D7E; margin-top: 8px; background: transparent; border: none;");
    headerLayout->addWidget(pageDescription);
    headerLayout->addSpacing(10);
    listLayout->addWidget(headerWidget);

    // --- Scrollable Vote List ---
    QScrollArea *scrollArea = new QScrollArea(votesListPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setStyleSheet(QString(
                                  "QScrollArea { background: %1; border: none; }"
                                  "QScrollBar:vertical {"
                                  "    border: none;"
                                  "    background: %1;"
                                  "    width: 10px;"
                                  "    margin: 0px;"
                                  "}"
                                  "QScrollBar::handle:vertical {"
                                  "    background: #B0BEC5;"
                                  "    min-height: 30px;"
                                  "    border-radius: 5px;"
                                  "}"
                                  "QScrollBar::handle:vertical:hover {"
                                  "    background: #90A4AE;"
                                  "}"
                                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                                  "    border: none; background: none; height: 0px;"
                                  "}"
                                  "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                  "    background: none;"
                                  "}"
                                  ).arg(bgColor));

    scrollContent = new QWidget(scrollArea);
    scrollContent->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(40, 20, 40, 30);
    scrollLayout->setSpacing(15);

    scrollLayout->addStretch();
    scrollArea->setWidget(scrollContent);
    listLayout->addWidget(scrollArea, 1);

    // Refresh the cards
    show_cards();
}

void MyVotesPage::createPollViewPage()
{
    pollViewPage = new QWidget(stackedWidget);
    pollViewPage->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    pollViewLayout = new QVBoxLayout(pollViewPage);
    pollViewLayout->setContentsMargins(40, 40, 40, 40);
    pollViewLayout->setSpacing(20);

    // Add a back button at the top
    QPushButton* backButton = new QPushButton("← Back to My Votes", pollViewPage);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; color: #007BFF; font-size: 16px; font-weight: bold; padding: 5px 0; text-align: left; }"
        "QPushButton:hover { color: #0056b3; }"
        );

    connect(backButton, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(votesListPage);
        show_cards();
    });

    pollViewLayout->addWidget(backButton);

    pollViewLayout->addStretch();
}

void MyVotesPage::onRemoveVoteClicked(const size_t& identifier)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Removal",
                                  QString("Are you sure you want to remove your vote?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Remove the vote from the database
        delete_user_vote(activeUser.id, identifier);

        // If we're in poll view, go back to the votes list
        if (stackedWidget->currentWidget() == pollViewPage) {
            stackedWidget->setCurrentWidget(votesListPage);
        }

        // Refresh the page to show updated votes
        show_cards();
    }
}

void MyVotesPage::onViewVotePollClicked(const size_t& identifier, RetrievePollDTO &selectedPoll)
{
    // Clear existing content in poll view
    QLayoutItem *item;
    while ((item = pollViewLayout->takeAt(1)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // Poll title
    QLabel* pollTitle = new QLabel(QString::fromStdString(selectedPoll.name), pollViewPage);
    pollTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: #2C3E50; margin-top: 10px;");
    pollTitle->setWordWrap(true);
    pollViewLayout->addWidget(pollTitle);

    // Poll description
    QLabel* pollDescription = new QLabel(QString::fromStdString(selectedPoll.desc), pollViewPage);
    pollDescription->setStyleSheet("font-size: 16px; color: #5D6D7E; margin-bottom: 15px;");
    pollDescription->setWordWrap(true);
    pollViewLayout->addWidget(pollDescription);

    // Section title for options
    QLabel* optionsTitle = new QLabel("Poll Options", pollViewPage);
    optionsTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #2C3E50; margin-bottom: 15px;");
    pollViewLayout->addWidget(optionsTitle);

    // Create scroll area for options
    QScrollArea* scrollArea = new QScrollArea(pollViewPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollContentWidget = new QWidget();
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContentWidget);
    scrollLayout->setContentsMargins(10, 10, 10, 10);
    scrollLayout->setSpacing(10);

    // Style scroll bar
    scrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #F1F1F1;"
        "    width: 8px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #B0B0B0;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #909090;"
        "}"
        );

    scrollArea->setWidget(scrollContentWidget);

    // Get the option that the user voted for
    size_t userVotedOptionId = 0;
    auto votes = retrieve_polls(activeUser.id);
    for (auto &vote : votes) {
        if (vote.poll.id == identifier) {
            userVotedOptionId = vote.option.id;
            break;
        }
    }

    // Display all options with the user's choice outlined
    for (const auto& option : selectedPoll.options) {
        QLabel* optionLabel = new QLabel(QString::fromStdString(option.name), pollViewPage);
        optionLabel->setWordWrap(true);
        optionLabel->setStyleSheet(
            option.id == userVotedOptionId ?
                "background-color: #E3F2FD; border: 2px solid #007BFF; border-radius: 8px; padding: 10px;" :
                "background-color: #FFFFFF; border: 1px solid #D0D0D0; border-radius: 8px; padding: 10px;"
            );

        QFont font = optionLabel->font();
        font.setPointSize(14);
        optionLabel->setFont(font);

        scrollLayout->addWidget(optionLabel);
    }

    pollViewLayout->addWidget(scrollArea);

    // Add a remove vote button at the bottom
    QPushButton* removeVoteBtn = new QPushButton("Remove My Vote", pollViewPage);
    removeVoteBtn->setCursor(Qt::PointingHandCursor);
    removeVoteBtn->setStyleSheet(QString(
                                     "QPushButton { background-color: %1; color: white; padding: 10px 20px; border: none; "
                                     "border-radius: 8px; font-weight: bold; font-size: 14px; margin-top: 20px; }"
                                     "QPushButton:hover { background-color: #C82333; }"
                                     "QPushButton:pressed { background-color: #BD2130; }"
                                     ).arg(dangerColor));

    // Connect remove button
    connect(removeVoteBtn, &QPushButton::clicked, this, [this, identifier]() {
        onRemoveVoteClicked(identifier);
    });

    pollViewLayout->addWidget(removeVoteBtn);
    pollViewLayout->addStretch();

    // Switch to poll view
    stackedWidget->setCurrentWidget(pollViewPage);
}

void MyVotesPage::show_cards()
{
    QLayoutItem *item;
    while ((item = scrollLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            // Delete the widget
            item->widget()->deleteLater();
        }
        // Delete the layout item itself
        delete item;
    }

    // --- Generate Vote Cards ---
    QList<voteData> voteHistory;

    auto votes = retrieve_polls(activeUser.id);
    for (auto &vote : votes)
    {
        voteHistory.push_back({vote.poll.id, vote.option.id, vote.poll.name, vote.option.name, get_user_by_id(vote.poll.owner_id).name});
    }

    if (voteHistory.isEmpty()) {
        // Show a message when there are no votes
        QLabel* noVotesLabel = new QLabel("You haven't voted on any polls yet.", scrollContent);
        noVotesLabel->setStyleSheet("font-size: 16px; color: #5D6D7E; padding: 20px;");
        noVotesLabel->setAlignment(Qt::AlignCenter);
        scrollLayout->addWidget(noVotesLabel);
    } else {
        for (const voteData &vote : voteHistory) {
            QFrame *voteCard = new QFrame(scrollContent);
            voteCard->setFrameShape(QFrame::StyledPanel);
            voteCard->setStyleSheet(QString(
                                        "QFrame { background-color: %1; border-radius: 8px; border: none; }"
                                        ).arg(cardBgColor));

            auto *cardLayout = new QVBoxLayout(voteCard);
            cardLayout->setContentsMargins(20, 20, 20, 20);
            cardLayout->setSpacing(15);

            auto *cardHeader = new QWidget(voteCard);
            cardHeader->setStyleSheet("background-color: transparent;");
            auto *headerLayout = new QHBoxLayout(cardHeader);
            headerLayout->setContentsMargins(0, 0, 0, 0);

            QLabel *pollTitleLabel = new QLabel(QString::fromStdString(vote.pollTitle), cardHeader);
            pollTitleLabel->setStyleSheet("background-color: transparent; font-size: 18px; font-weight: bold; color: #2C3E50;");
            pollTitleLabel->setWordWrap(true);
            headerLayout->addWidget(pollTitleLabel, 1);

            headerLayout->addSpacing(15);

            cardLayout->addWidget(cardHeader);

            // Card Body (Vote details & actions)
            auto *cardBody = new QWidget(voteCard);
            cardBody->setStyleSheet("background-color: transparent;");
            auto *bodyLayout = new QHBoxLayout(cardBody);
            bodyLayout->setContentsMargins(0, 10, 0, 0);
            bodyLayout->setSpacing(20);

            // Left side: Your Vote & Creator Info
            auto *infoContainer = new QWidget(cardBody);
            infoContainer->setStyleSheet("background-color: transparent;");
            auto *infoLayout = new QVBoxLayout(infoContainer);
            infoLayout->setContentsMargins(0,0,0,0);
            infoLayout->setSpacing(10);

            // Your Vote Info
            QWidget *yourVoteWidget = new QWidget(infoContainer);
            yourVoteWidget->setStyleSheet("background-color: transparent;");
            QVBoxLayout *yourVoteLayout = new QVBoxLayout(yourVoteWidget);
            yourVoteLayout->setContentsMargins(0,0,0,0);
            yourVoteLayout->setSpacing(2);
            QLabel *yourVoteLabel = new QLabel("Your Vote:", yourVoteWidget);
            yourVoteLabel->setStyleSheet("background-color: transparent; font-size: 14px; color: #6C757D;");
            QLabel *voteOptionLabel = new QLabel(QString::fromStdString(vote.votedOption), yourVoteWidget);
            voteOptionLabel->setStyleSheet(QString("background-color: transparent; font-size: 16px; color: %1; font-weight: bold;").arg(accentColor));
            voteOptionLabel->setWordWrap(true);
            yourVoteLayout->addWidget(yourVoteLabel);
            yourVoteLayout->addWidget(voteOptionLabel);
            infoLayout->addWidget(yourVoteWidget);

            // Creator Info
            QWidget *creatorWidget = new QWidget(infoContainer);
            creatorWidget->setStyleSheet("background-color: transparent;");
            QVBoxLayout *creatorLayout = new QVBoxLayout(creatorWidget);
            creatorLayout->setContentsMargins(0,0,0,0);
            creatorLayout->setSpacing(2);
            QLabel *creatorLabel = new QLabel("Created by:", creatorWidget);
            creatorLabel->setStyleSheet("background-color: transparent; font-size: 14px; color: #6C757D;");
            QLabel *creatorNameLabel = new QLabel(QString::fromStdString(vote.pollCreator), creatorWidget);
            creatorNameLabel->setStyleSheet("background-color: transparent; font-size: 16px; color: #2C3E50;");
            creatorNameLabel->setWordWrap(true);
            creatorLayout->addWidget(creatorLabel);
            creatorLayout->addWidget(creatorNameLabel);
            infoLayout->addWidget(creatorWidget);

            infoLayout->addStretch();

            bodyLayout->addWidget(infoContainer, 1);

            // Right side: Buttons (View Poll, Remove Vote)
            auto *buttonContainer = new QWidget(cardBody);
            buttonContainer->setStyleSheet("background-color: transparent;");
            auto *buttonLayout = new QVBoxLayout(buttonContainer);
            buttonLayout->setContentsMargins(0, 0, 0, 0);
            buttonLayout->setSpacing(8);
            buttonLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

            QPushButton *viewBtn = new QPushButton("View Poll", buttonContainer);
            viewBtn->setCursor(Qt::PointingHandCursor);
            viewBtn->setStyleSheet(QString(
                                       "QPushButton { background-color: %1; color: white; padding: 10px 18px; border: none; border-radius: 8px; font-weight: bold; font-size: 14px; }"
                                       "QPushButton:hover { background-color: %2; }"
                                       "QPushButton:pressed { background-color: #0056b3; }"
                                       ).arg(primaryColor,primaryHover));
            viewBtn->setFixedWidth(120);

            // Store poll ID for the view button
            size_t pollId = vote.pollId;

            // Connect button using a direct method to retrieve poll data at click time
            connect(viewBtn, &QPushButton::clicked, this, [this, pollId]() {
                // Fetch the poll data at the time of clicking to ensure it's fresh
                RetrievePollDTO pollData = retrieve_public_poll(pollId);
                onViewVotePollClicked(pollId, pollData);
            });

            // Remove Vote Button
            QPushButton *removeBtn = new QPushButton("Remove", buttonContainer);
            removeBtn->setCursor(Qt::PointingHandCursor);
            removeBtn->setStyleSheet(QString(
                                         "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                                         "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                                         "QPushButton:pressed { background-color: %1; color: white; }"
                                         ).arg(dangerColor).arg(dangerHoverBg).arg(dangerHoverFg));
            removeBtn->setFixedWidth(120);

            // Connect remove button
            connect(removeBtn, &QPushButton::clicked, this, [this, pollId]() {
                onRemoveVoteClicked(pollId);
            });

            buttonLayout->addWidget(viewBtn);
            buttonLayout->addWidget(removeBtn);
            buttonLayout->addStretch();

            bodyLayout->addWidget(buttonContainer);

            cardLayout->addWidget(cardBody);

            scrollLayout->addWidget(voteCard);
        } // End of vote card loop
    }

    // Add stretch at the end to push cards to the top
    scrollLayout->addStretch();
}
