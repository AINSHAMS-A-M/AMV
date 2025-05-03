#include "mypolls.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QSpacerItem>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QDatetime>
#include <QMessageBox>
#include "db.hpp"
#include "mainwindow.h"
#include "services.hpp"
#include "sidebar.h"

MyPollsPage::MyPollsPage(QWidget *parent)
    : QWidget(parent),
    pollListView(nullptr),
    contentStack(nullptr),
    sidebar(nullptr),
    content(nullptr),
    pollDetailsView(nullptr),
    pollListScrollArea(nullptr),
    pollListContainerWidget(nullptr),
    pollListLayout(nullptr),
    pollDetailsLayout(nullptr),
    backButton(nullptr),
    pollDetailsTitleLabel(nullptr),
    optionsTable(nullptr)
{
    const QString bgColor       = "#F5F6F8";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this,"My Polls");


    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(20);
    contentStack = new QStackedWidget(content);
    contentLayout->addWidget(contentStack);
    setupPollListView();
    setupPollDetailsView();

    // Add the content widget to the root layout
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
    rootLayout->setStretch(1, 1);

    // Populate the poll list when the page is created
    populatePollList();
}

void MyPollsPage::setupPollListView() {
    pollListView = new QWidget(contentStack);
    pollListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *pageLayout = new QVBoxLayout(pollListView);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(15);
    QLabel *titleLabel = new QLabel("My Created Polls", pollListView);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333; margin-bottom: 10px;");
    pageLayout->addWidget(titleLabel);
    pollListScrollArea = new QScrollArea(pollListView);
    pollListScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pollListScrollArea->setWidgetResizable(true);
    pollListScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    pollListScrollArea->setStyleSheet(R"(
        /* Container background */
        QScrollArea {
            border: none;
            background-color: #F5F6F8;
        }
        /* Vertical scrollbar track */
        QScrollBar:vertical {
            background: transparent;
            width: 12px;
            margin: 0px;
        }
        /* Scrollbar handle */
        QScrollBar::handle:vertical {
            background: #CCCCCC;
            min-height: 20px;
            border-radius: 6px;
        }
        /* Hover state */
        QScrollBar::handle:vertical:hover {
            background: #AAAAAA;
        }
        /* Remove the arrows and lines */
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: transparent;
        }
    )");
    pollListContainerWidget = new QWidget();
    pollListLayout = new QVBoxLayout(pollListContainerWidget);
    pollListLayout->setContentsMargins(0, 0, 0, 0);
    pollListLayout->setSpacing(10);
    pollListLayout->setAlignment(Qt::AlignTop);
    pollListScrollArea->setWidget(pollListContainerWidget);
    pageLayout->addWidget(pollListScrollArea);
    contentStack->addWidget(pollListView);
}

void MyPollsPage::setupPollDetailsView() {
    pollDetailsView = new QWidget(contentStack);
    pollDetailsLayout = new QVBoxLayout(pollDetailsView);
    pollDetailsLayout->setContentsMargins(0, 0, 0, 0);
    pollDetailsLayout->setSpacing(15);

    // Header layout for back button and title
    auto *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(15);

    backButton = new QPushButton("← Back", pollDetailsView);
    backButton->setStyleSheet(QString(
        "QPushButton { color: #FFFFFF; background-color: #007BFF; border: none; padding: 8px 15px; border-radius: 5px; font-size: 16px; }"
        "QPushButton:hover { background-color: #339CFF; }"
        ));
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setFixedWidth(100);
    connect(backButton, &QPushButton::clicked, this, &MyPollsPage::onBackToListClicked);
    headerLayout->addWidget(backButton, 0, Qt::AlignLeft | Qt::AlignTop);

    pollDetailsTitleLabel = new QLabel("Poll Title", pollDetailsView);
    pollDetailsTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    pollDetailsTitleLabel->setWordWrap(true);
    headerLayout->addWidget(pollDetailsTitleLabel, 1, Qt::AlignTop);

    pollDetailsLayout->addLayout(headerLayout);

    // Table for options and votes
    optionsTable = new QTableWidget(pollDetailsView);
    optionsTable->setColumnCount(2);
    optionsTable->setHorizontalHeaderLabels({"Option Text", "Votes"});
    optionsTable->horizontalHeader()->setStyleSheet(QString(
        "QHeaderView::section {"
        "background-color: #F0F0F0;"
        "color: #333333;"
        "padding: 8px;"
        "border: 1px solid #DDDDDD;"
        "font-size: 16px;"
        "font-weight: bold;"
        "}"
        ));
    optionsTable->verticalHeader()->setVisible(false);
    optionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    optionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    optionsTable->setSelectionMode(QAbstractItemView::NoSelection);
    optionsTable->setStyleSheet(QString(R"(
        QTableWidget {
            background-color: #FFFFFF;
            border: 1px solid #DDDDDD;
            border-radius: 6px;
            font-size: 18px;
            gridline-color: #EEEEEE;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #F0F0F0;
        }
        QTableWidget:focus { outline: 0px; }
        QScrollBar:vertical {
            background: #F5F6F8;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #C0C0C0;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #A0A0A0;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: transparent;
        }
    )"));

    optionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    optionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    optionsTable->setColumnWidth(1, 130);

    pollDetailsLayout->addWidget(optionsTable);
    contentStack->addWidget(pollDetailsView);
}

void MyPollsPage::populatePollList() {
    // Clear existing cards from the layout
    QLayoutItem *item;
    while ((item = pollListLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    pollListLayout->addStretch();
    auto myPolls = show_created_polls(activeUser.id);
    if (!myPolls.size()) {
        QLabel *noPollsLabel = new QLabel("You haven't created any polls yet.", pollListContainerWidget);
        noPollsLabel->setStyleSheet("font-size: 18px; color: #666666; margin-top: 50px;");
        noPollsLabel->setAlignment(Qt::AlignCenter);
        pollListLayout->addWidget(noPollsLabel);
    } else {
        for (const auto& id : myPolls) {
            auto poll = retrieve_poll_as_owner(activeUser.id, id);

            // Create a widget for the poll card
            QWidget *cardWidget = new QWidget(pollListContainerWidget);
            cardWidget->setStyleSheet(QString(
                "QWidget { background-color: #FFFFFF; border-radius: 8px; }"
                ));

            // Set up vertical layout for the entire card
            QVBoxLayout *cardOuterLayout = new QVBoxLayout(cardWidget);
            cardOuterLayout->setContentsMargins(15, 15, 15, 15);
            cardOuterLayout->setSpacing(10);

            // Title and description layout
            QVBoxLayout *contentLayout = new QVBoxLayout();
            contentLayout->setSpacing(5);

            // Poll title
            QLabel *pollTitleLabel = new QLabel(QString::fromStdString(poll.pollInfo.name), cardWidget);
            pollTitleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333;");
            pollTitleLabel->setWordWrap(true);
            contentLayout->addWidget(pollTitleLabel);

            // Poll description
            QLabel *pollDescriptionLabel = new QLabel(QString::fromStdString(poll.pollInfo.desc), cardWidget);
            pollDescriptionLabel->setStyleSheet("font-size: 14px; color: #666666;");
            pollDescriptionLabel->setWordWrap(true);

            // Only add height if there's content to display
            if (!poll.pollInfo.desc.empty()) {
                pollDescriptionLabel->setMinimumHeight(20);
                // For very short descriptions, keep the card compact
                if (poll.pollInfo.desc.length() < 30) {
                    pollDescriptionLabel->setMaximumHeight(40);
                }
            } else {
                pollDescriptionLabel->setMaximumHeight(0);
            }

            contentLayout->addWidget(pollDescriptionLabel);

            // Buttons layout
            QHBoxLayout *buttonsLayout = new QHBoxLayout();
            buttonsLayout->setSpacing(10);

            // Spacer to push buttons to the right
            buttonsLayout->addStretch(1);

            // Delete button
            QPushButton *deleteButton = new QPushButton("Delete", cardWidget);
            deleteButton->setStyleSheet(QString(
                                            "QPushButton { color: %1; background-color: %2; border: none; padding: 8px 20px; border-radius: 5px; font-size: 16px; }"
                                            "QPushButton:hover { background-color: %3; }"
                                            ).arg("#FFFFFF", "#DC3545", "#E25563"));
            deleteButton->setCursor(Qt::PointingHandCursor);
            deleteButton->setFixedWidth(100);
            buttonsLayout->addWidget(deleteButton);

            // View button
            QPushButton *viewButton = new QPushButton("View", cardWidget);
            viewButton->setStyleSheet(QString(
                                          "QPushButton { color: %1; background-color: %2; border: none; padding: 8px 20px; border-radius: 5px; font-size: 16px; }"
                                          "QPushButton:hover { background-color: %3; }"
                                          ).arg("#FFFFFF", "#007BFF", "#339CFF"));
            viewButton->setCursor(Qt::PointingHandCursor);
            viewButton->setFixedWidth(100);
            buttonsLayout->addWidget(viewButton);

            // Add the layouts to the main card layout
            cardOuterLayout->addLayout(contentLayout, 1);
            cardOuterLayout->addLayout(buttonsLayout);

            // Add the card to the list
            pollListLayout->addWidget(cardWidget);

            // Connect button signals
            connect(viewButton, &QPushButton::clicked, this, [this, id]() {
                onViewPollClicked(id);
            });

            connect(deleteButton, &QPushButton::clicked, this, [this, id]() {
                onDeletePollClicked(id);
            });
        }
    }
    pollListLayout->addStretch();
}

void MyPollsPage::onBackToListClicked() {
    contentStack->setCurrentWidget(pollListView);
    optionsTable->clearContents();
    optionsTable->setRowCount(0);
    pollDetailsTitleLabel->setText("Poll Title");
}

void MyPollsPage::displayPollDetails(const RetrievePollResultAdmin& poll) {
    pollDetailsTitleLabel->setText(QString::fromStdString(poll.pollInfo.name));

    // Populate the options table
    optionsTable->clearContents();
    optionsTable->setRowCount(poll.results.size());
    for (size_t i = 0; i < poll.results.size(); ++i) {
        const auto& option = poll.results[i];
        QTableWidgetItem *textItem = new QTableWidgetItem(QString::fromStdString(option.option_name));
        QTableWidgetItem *votesItem = new QTableWidgetItem(QString::number(option.option_votes_count));
        votesItem->setTextAlignment(Qt::AlignCenter);
        optionsTable->setItem(i, 0, textItem);
        optionsTable->setItem(i, 1, votesItem);
    }

    optionsTable->resizeColumnsToContents();
    optionsTable->resizeRowsToContents();
    optionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    optionsTable->setColumnWidth(1, 120);
}

void MyPollsPage::onViewPollClicked(size_t pollId) {
    // Fetch the details for this specific poll
    auto pollDetails = retrieve_poll_as_owner(activeUser.id, pollId);
    if (pollDetails.pollInfo.id == pollId) // Basic check
    {
        displayPollDetails(pollDetails); // Populate the detailed view
        contentStack->setCurrentWidget(pollDetailsView); // Switch to the detailed view page
    }
}

void MyPollsPage::onDeletePollClicked(size_t pollId)
{
    // Confirmation dialog
    QMessageBox confirmBox;
    confirmBox.setWindowTitle("Confirm Delete");
    confirmBox.setText("Are you sure you want to delete this poll?");
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setDefaultButton(QMessageBox::No);
    confirmBox.setIcon(QMessageBox::Question);

    int result = confirmBox.exec();
    if (result == QMessageBox::Yes)
    {
        // Call the delete poll service
        delete_poll(pollId);
        // Refresh the poll list
        populatePollList();
    }
}
