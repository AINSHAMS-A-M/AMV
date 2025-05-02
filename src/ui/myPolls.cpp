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
#include "db.hpp"
#include "mainwindow.h"
#include "services.hpp"
#include "sidebar.h"

MyPollsPage::MyPollsPage(QWidget *parent)
    : QWidget(parent),
    sidebar(nullptr),
    content(nullptr),
    contentStack(nullptr),
    pollListView(nullptr),
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
    rootLayout->setStretch(0, 0); // Sidebar fixed width
    rootLayout->setStretch(1, 1); // Content takes remaining space
    // Populate the poll list when the page is created
    populatePollList();
}

void MyPollsPage::setupPollListView() {
    pollListView = new QWidget(contentStack);
    pollListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *pageLayout = new QVBoxLayout(pollListView);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(15); // Spacing between poll cards
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
    pollListContainerWidget = new QWidget(); // The actual container for cards
    pollListLayout = new QVBoxLayout(pollListContainerWidget);
    pollListLayout->setContentsMargins(0, 0, 0, 0);
    pollListLayout->setSpacing(10); // Spacing between poll cards inside container
    pollListLayout->setAlignment(Qt::AlignTop); // Align cards to the top
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

    // Improved header styling
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

    // Enhanced table styling with larger text
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
    optionsTable->setColumnWidth(1, 130); // Compact width for vote counts

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
    pollListLayout->addStretch(); // Add the stretch back after clearing
    auto myPolls = show_created_polls(activeUser.id);
    if (!myPolls.size()) {
        QLabel *noPollsLabel = new QLabel("You haven't created any polls yet.", pollListContainerWidget);
        noPollsLabel->setStyleSheet("font-size: 18px; color: #666666; margin-top: 50px;");
        noPollsLabel->setAlignment(Qt::AlignCenter);
        pollListLayout->addWidget(noPollsLabel);
    } else {
        for (const auto& id : myPolls) {
            auto poll =  retrieve_poll_as_owner(activeUser.id,id);
            // Create a widget for the poll card
            QWidget *cardWidget = new QWidget(pollListContainerWidget);
            QHBoxLayout *cardLayout = new QHBoxLayout(cardWidget);
            cardLayout->setContentsMargins(15, 15, 15, 15);
            cardLayout->setSpacing(15);
            cardWidget->setStyleSheet(QString(
                "QWidget { background-color: #FFFFFF; border-radius: 8px; }"
                )
                                      );
            QLabel *pollTitleLabel = new QLabel(QString::fromStdString(poll.pollInfo.name), cardWidget);
            pollTitleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333;");
            pollTitleLabel->setWordWrap(true);
            cardLayout->addWidget(pollTitleLabel, 1); // Title takes most space
            QPushButton *viewButton = new QPushButton("View", cardWidget);
            viewButton->setStyleSheet(QString(
                                          "QPushButton { color: %1; background-color: %2; border: none; padding: 8px 20px; border-radius: 5px; font-size: 16px; }"
                                          "QPushButton:hover { background-color: %3; }"
                                          ).arg("#FFFFFF", "#007BFF", "#339CFF")); // White text, Primary Blue background
            viewButton->setCursor(Qt::PointingHandCursor);
            viewButton->setFixedWidth(100); // Fixed width for view button
            cardLayout->addWidget(viewButton);
            pollListLayout->addWidget(cardWidget);
            connect(viewButton, &QPushButton::clicked, this, [this, id]() {
                onViewPollClicked(id);
            });
        }
    }
    pollListLayout->addStretch();
}

void MyPollsPage::onBackToListClicked() {
    contentStack->setCurrentWidget(pollListView); // Switch back to the list page
    // Optionally clear the details view or reset scroll position on the list
    optionsTable->clearContents(); // Clear table data
    optionsTable->setRowCount(0);
    pollDetailsTitleLabel->setText("Poll Title"); // Reset title
}

void MyPollsPage::displayPollDetails(const RetrievePollResultAdmin& poll) {
    pollDetailsTitleLabel->setText(QString::fromStdString(poll.pollInfo.name)); // Set the poll title
    // Populate the options table
    optionsTable->clearContents(); // Clear previous data
    optionsTable->setRowCount(poll.results.size()); // Set number of rows
    for (size_t i = 0; i < poll.results.size(); ++i) {
        const auto& option = poll.results[i];
        QTableWidgetItem *textItem = new QTableWidgetItem(QString::fromStdString(option.option_name));
        QTableWidgetItem *votesItem = new QTableWidgetItem(QString::number(option.option_votes_count));
        votesItem->setTextAlignment(Qt::AlignCenter); // Align vote count to center
        optionsTable->setItem(i, 0, textItem);
        optionsTable->setItem(i, 1, votesItem);
    }
    // Auto-size columns for better fit
    optionsTable->resizeColumnsToContents();
    optionsTable->resizeRowsToContents();
    // Make the first column (Option Text) stretch
    optionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    optionsTable->setColumnWidth(1, 120);
    // optionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
}

void MyPollsPage::onViewPollClicked(size_t pollId) {
    // Fetch the details for this specific poll
    auto pollDetails = retrieve_poll_as_owner(activeUser.id,pollId);
    if (pollDetails.pollInfo.id == pollId) // Basic check
    {
        displayPollDetails(pollDetails); // Populate the detailed view
        contentStack->setCurrentWidget(pollDetailsView); // Switch to the detailed view page
    }
}
