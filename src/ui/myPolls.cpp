#include "db.hpp"
#include "mainwindow.h"
#include "mypolls.h"
#include "services.hpp"
#include "sidebar.h"
#include <QDatetime>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

MyPollsPage::MyPollsPage(QWidget* parent)
    : QWidget(parent)
    , pollListView(nullptr)
    , contentStack(nullptr)
    , sidebar(nullptr)
    , content(nullptr)
    , pollDetailsView(nullptr)
    , pollListScrollArea(nullptr)
    , pollListContainerWidget(nullptr)
    , pollListLayout(nullptr)
    , pollDetailsLayout(nullptr)
    , backButton(nullptr)
    , pollDetailsTitleLabel(nullptr)
    , optionsTable(nullptr)
    , editDescriptionView(nullptr)
    , customizeOptionsView(nullptr)
    , editDescriptionLayout(nullptr)
    , editDescriptionTitleLabel(nullptr)
    , descriptionTextEdit(nullptr)
    , saveDescriptionButton(nullptr)
    , cancelEditDescriptionButton(nullptr)
    , customizeOptionsLayout(nullptr)
    , customizeOptionsTitleLabel(nullptr)
    , editableOptionsTable(nullptr)
    , votersPage(nullptr)
    , addOptionButton(nullptr)
    , saveOptionsButton(nullptr)
    , cancelCustomizeOptionsButton(nullptr)
    , votersTree(nullptr)
{
    auto* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this, "My Polls");

    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(20);

    contentStack = new QStackedWidget(content);
    contentLayout->addWidget(contentStack);

    setupPollListView();
    setupPollDetailsView();
    setupEditDescriptionView();
    setupCustomizeOptionsView();
    setupVotersView();

    // Add content widget to layout
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
    rootLayout->setStretch(1, 1);

    // Populate poll list on startup
    populatePollList();
}

void MyPollsPage::setupPollListView()
{
    pollListView = new QWidget(contentStack);
    pollListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* pageLayout = new QVBoxLayout(pollListView);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setSpacing(15);

    QLabel* titleLabel = new QLabel("My Created Polls", pollListView);
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
        QScrollBar::handle:vertical:hover {
            background: #AAAAAA;
        }
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

void MyPollsPage::setupPollDetailsView()
{
    pollDetailsView = new QWidget(contentStack);
    pollDetailsLayout = new QVBoxLayout(pollDetailsView);
    pollDetailsLayout->setContentsMargins(0, 0, 0, 0);
    pollDetailsLayout->setSpacing(15);

    // Header layout for back button and title
    auto* headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(15);

    backButton = new QPushButton("← Back to My Polls", pollDetailsView);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton { background: none; border: none; color: #007BFF; font-size: 16px; font-weight: bold; padding: 5px 0; text-align: left; }"
        "QPushButton:hover { color: #0056b3; }"
    );
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
    optionsTable->setHorizontalHeaderLabels({ "Option", "Votes" });
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

void MyPollsPage::populatePollList()
{
    // Clear existing cards from layout
    QLayoutItem* item;
    while ((item = pollListLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            item->widget()->deleteLater();
        }
        delete item;
    }

    pollListLayout->addStretch();

    auto myPolls = show_created_polls(activeUser.id);

    if (!myPolls.size())
    {
        QLabel* noPollsLabel = new QLabel("You haven't created any polls yet.", pollListContainerWidget);
        noPollsLabel->setStyleSheet("font-size: 18px; color: #666666; margin-top: 50px;");
        noPollsLabel->setAlignment(Qt::AlignCenter);
        pollListLayout->addWidget(noPollsLabel);
    }
    else
    {
        for (const auto& id : myPolls)
        {
            auto poll = retrieve_poll_results(id);
            bool isDone = poll.pollInfo.is_finished;

            // Create a widget for the poll card
            QWidget* cardWidget = new QWidget(pollListContainerWidget);
            cardWidget->setObjectName("pollCard");
            cardWidget->setStyleSheet(QString(
                "QWidget {"
                "  background-color: #FFFFFF;"
                "  border-radius: 8px;}"
                "#pollCard {"
                "  background-color: #FFFFFF;"
                "  border-radius: 8px;"
                "  border: %1;"
                "}"
            ).arg(isDone ? "1px solid #DDDDDD" : "2px solid #27AE60"));

            // Card outer layout
            QVBoxLayout* cardOuterLayout = new QVBoxLayout(cardWidget);
            cardOuterLayout->setContentsMargins(15, 15, 15, 15);
            cardOuterLayout->setSpacing(10);

            // Content layout
            QVBoxLayout* contentLayout = new QVBoxLayout();
            contentLayout->setSpacing(5);

            // Poll title
            QLabel* pollTitleLabel = new QLabel(QString::fromStdString(poll.pollInfo.name), cardWidget);
            pollTitleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333;");
            pollTitleLabel->setWordWrap(true);
            contentLayout->addWidget(pollTitleLabel);

            // State badge
            QLabel* stateBadge = new QLabel(isDone ? "Finished" : "Ongoing", cardWidget);
            stateBadge->setStyleSheet(QString(
                "font-size: 12px;"
                "font-weight: bold;"
                "color: %1;"
            ).arg(isDone ? "#7F8C8D" : "#27AE60"));
            contentLayout->addWidget(stateBadge);

            // Description label
            QLabel* pollDescriptionLabel = new QLabel(QString::fromStdString(poll.pollInfo.desc), cardWidget);
            pollDescriptionLabel->setStyleSheet("font-size: 14px; color: #666666;");
            pollDescriptionLabel->setWordWrap(true);

            if (!poll.pollInfo.desc.empty())
            {
                pollDescriptionLabel->setMinimumHeight(20);
                if (poll.pollInfo.desc.length() < 30)
                {
                    pollDescriptionLabel->setMaximumHeight(40);
                }
            }
            else
            {
                pollDescriptionLabel->setMaximumHeight(0);
            }

            contentLayout->addWidget(pollDescriptionLabel);

            // Buttons layout
            QHBoxLayout* buttonsLayout = new QHBoxLayout();
            buttonsLayout->setSpacing(10);
            buttonsLayout->addStretch(1);

            // End Election Button
            QPushButton* endElectionButton = new QPushButton("End Election", cardWidget);
            endElectionButton->setStyleSheet(QString(
                "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                "QPushButton:pressed { background-color: %1; color: white; }"
            ).arg(dangerColor, dangerHoverBg, dangerHoverFg));
            endElectionButton->setCursor(Qt::PointingHandCursor);
            endElectionButton->setFixedWidth(120);

            connect(endElectionButton, &QPushButton::clicked, this, [this, id]()
            {
                auto reply = QMessageBox::question(
                    this,
                    "Confirm End Election",
                    QString("Are you sure you want to end this poll?"),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                );

                if (reply == QMessageBox::Yes)
                {
                    if (endPoll(id))
                    {
                        populatePollList();
                        QMessageBox::information(
                            this,
                            "Election Ended",
                            QString("Poll has been successfully ended!")
                        );
                    }
                    else
                    {
                        QMessageBox::warning(
                            this,
                            "Warning",
                            QString("Poll is already ended!")
                        );
                    }
                }
            });

            buttonsLayout->addWidget(endElectionButton);

            // Delete Button
            QPushButton* deleteButton = new QPushButton("Delete", cardWidget);
            deleteButton->setStyleSheet(QString(
                "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                "QPushButton:pressed { background-color: %1; color: white; }"
            ).arg(dangerColor, dangerHoverBg, dangerHoverFg));
            deleteButton->setCursor(Qt::PointingHandCursor);
            deleteButton->setFixedWidth(100);

            buttonsLayout->addWidget(deleteButton);

            // Edit Description Button
            QPushButton* editDescriptionButton = new QPushButton("Edit Description", cardWidget);
            editDescriptionButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
            ));
            editDescriptionButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(editDescriptionButton);

            connect(editDescriptionButton, &QPushButton::clicked, this, [this, id]() {
                onEditDescriptionClicked(id);
            });

            // Customize Options Button
            QPushButton* customizeOptionsButton = new QPushButton("Customize Options", cardWidget);
            customizeOptionsButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
            ));
            customizeOptionsButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(customizeOptionsButton);

            connect(customizeOptionsButton, &QPushButton::clicked, this, [this, id]() {
                onCustomizeOptionsClicked(id);
            });

            // See Voters Button
            QPushButton* seeVotersButton = new QPushButton("See Voters", cardWidget);
            seeVotersButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
            ));
            seeVotersButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(seeVotersButton);

            connect(seeVotersButton, &QPushButton::clicked, this, [this, id]() {
                onSeeVotersClicked(id);
            });

            // View Button
            QPushButton* viewButton = new QPushButton("View", cardWidget);
            viewButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
            ));
            viewButton->setCursor(Qt::PointingHandCursor);
            viewButton->setFixedWidth(100);

            buttonsLayout->addWidget(viewButton);

            // Add layouts to main card layout
            cardOuterLayout->addLayout(contentLayout, 1);
            cardOuterLayout->addLayout(buttonsLayout);

            // Add card to layout
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

void MyPollsPage::onBackToListClicked()
{
    // Switch back to poll list view
    contentStack->setCurrentWidget(pollListView);

    // Clear table contents
    optionsTable->clearContents();
    optionsTable->setRowCount(0);

    // Reset title label
    pollDetailsTitleLabel->setText("Poll Title");
}

void MyPollsPage::displayPollDetails(const RetrievePollResultAdmin& poll)
{
    // Set poll title in detail view
    pollDetailsTitleLabel->setText(QString::fromStdString(poll.pollInfo.name));

    // Populate results into the table
    optionsTable->clearContents();
    optionsTable->setRowCount(poll.results.size());

    for (size_t i = 0; i < poll.results.size(); ++i)
    {
        const auto& option = poll.results[i];

        QTableWidgetItem* textItem = new QTableWidgetItem(QString::fromStdString(option.option_name));
        QTableWidgetItem* votesItem = new QTableWidgetItem(QString::number(option.option_votes_count));
        votesItem->setTextAlignment(Qt::AlignCenter);

        optionsTable->setItem(i, 0, textItem);
        optionsTable->setItem(i, 1, votesItem);
    }

    // Adjust layout and styling
    optionsTable->resizeColumnsToContents();
    optionsTable->resizeRowsToContents();
    optionsTable->setStyleSheet("color: #4A5568;");
    optionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    optionsTable->setColumnWidth(1, 120);
}

void MyPollsPage::onViewPollClicked(size_t pollId)
{
    // Fetch poll details
    auto pollDetails = retrieve_poll_results(pollId);

    if (pollDetails.pollInfo.id == pollId) // Basic validation
    {
        displayPollDetails(pollDetails); // Show detailed results
        contentStack->setCurrentWidget(pollDetailsView); // Switch to details view
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
        delete_poll(pollId); // Perform deletion
        populatePollList();   // Refresh list
    }
}

void MyPollsPage::setupEditDescriptionView()
{
    editDescriptionView = new QWidget(contentStack);
    editDescriptionLayout = new QVBoxLayout(editDescriptionView);
    editDescriptionLayout->setContentsMargins(0, 250, 0, 0);
    editDescriptionLayout->setSpacing(50);
    editDescriptionLayout->setAlignment(Qt::AlignCenter);

    // Title label
    editDescriptionTitleLabel = new QLabel("Edit Poll Description", editDescriptionView);
    editDescriptionTitleLabel->setAlignment(Qt::AlignCenter);
    editDescriptionTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    editDescriptionLayout->addWidget(editDescriptionTitleLabel);

    // Description input field
    descriptionTextEdit = new QLineEdit(editDescriptionView);
    descriptionTextEdit->setPlaceholderText("Enter poll description...");
    descriptionTextEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "color: #4A5568;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}"
        );
    descriptionTextEdit->setMinimumHeight(50);
    editDescriptionLayout->addWidget(descriptionTextEdit);

    // Action buttons
    auto* buttonLayout = new QHBoxLayout();

    cancelEditDescriptionButton = new QPushButton("Cancel", editDescriptionView);
    cancelEditDescriptionButton->setStyleSheet(QString(
                                                   "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                                                   "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                                                   "QPushButton:pressed { background-color: %1; color: white; }"
                                                   ).arg(dangerColor, dangerHoverBg, dangerHoverFg));
    cancelEditDescriptionButton->setCursor(Qt::PointingHandCursor);
    cancelEditDescriptionButton->setMinimumWidth(60);
    connect(cancelEditDescriptionButton, &QPushButton::clicked, this, &MyPollsPage::onCancelEditDescriptionClicked);
    buttonLayout->addWidget(cancelEditDescriptionButton);

    saveDescriptionButton = new QPushButton("Save Description", editDescriptionView);
    saveDescriptionButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: #489978;"
        "  color: white;"
        "  padding: 6px 10px;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #38A169;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2F855A;"
        "}"
        ));
    saveDescriptionButton->setCursor(Qt::PointingHandCursor);
    connect(saveDescriptionButton, &QPushButton::clicked, this, &MyPollsPage::onSaveDescriptionClicked);
    connect(descriptionTextEdit, &QLineEdit::returnPressed, this, &MyPollsPage::onSaveDescriptionClicked);
    buttonLayout->addWidget(saveDescriptionButton);

    editDescriptionLayout->addLayout(buttonLayout);
    editDescriptionLayout->addStretch();

    contentStack->addWidget(editDescriptionView);
    descriptionTextEdit->setFocus();
}

void MyPollsPage::setupCustomizeOptionsView()
{
    customizeOptionsView = new QWidget(contentStack);
    customizeOptionsLayout = new QVBoxLayout(customizeOptionsView);
    customizeOptionsLayout->setContentsMargins(0, 100, 0, 50);
    customizeOptionsLayout->setSpacing(15);

    // Title
    customizeOptionsTitleLabel = new QLabel("Customize Poll Options", customizeOptionsView);
    customizeOptionsTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    customizeOptionsLayout->addWidget(customizeOptionsTitleLabel);

    // Description label
    QLabel* descriptionLabel = new QLabel("Add, edit, or remove options for your poll below.", customizeOptionsView);
    descriptionLabel->setStyleSheet("font-size: 16px; color: #666666; margin-bottom: 10px;");
    customizeOptionsLayout->addWidget(descriptionLabel);

    // Add new option section
    QWidget* addOptionWidget = new QWidget(customizeOptionsView);
    addOptionWidget->setObjectName("addOptionWidget");
    addOptionWidget->setStyleSheet(
        "#addOptionWidget {"
        "  padding: 0px;"
        "  margin-bottom: 15px;"
        "}"
        );

    QHBoxLayout* addOptionLayout = new QHBoxLayout(addOptionWidget);
    addOptionLayout->setContentsMargins(0, 0, 0, 0);
    addOptionLayout->setSpacing(12);

    // Input field
    QLineEdit* newOptionInput = new QLineEdit(addOptionWidget);
    newOptionInput->setPlaceholderText("Enter new option...");
    newOptionInput->setStyleSheet(
        "QLineEdit {"
        "  border: 1px solid #D1D5DB;"
        "  border-radius: 6px;"
        "  padding: 10px 12px;"
        "  font-size: 15px;"
        "  color: #333333;"
        "  background-color: white;"
        "  margin: 0px;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #3498DB;"
        "  outline: none;"
        "}"
        );

    // Add button
    QPushButton* addOptionButton = new QPushButton("Add Option", addOptionWidget);
    addOptionButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #2ECC71;"
        "  color: white;"
        "  padding: 10px 20px;"
        "  border-radius: 6px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #27AE60;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1D8348;"
        "}"
        );
    addOptionButton->setCursor(Qt::PointingHandCursor);

    addOptionLayout->addWidget(newOptionInput);
    addOptionLayout->addWidget(addOptionButton);

    customizeOptionsLayout->addWidget(addOptionWidget);

    // Table for existing options
    editableOptionsTable = new QTableWidget(customizeOptionsView);
    editableOptionsTable->setColumnCount(2);
    editableOptionsTable->setHorizontalHeaderLabels({ "Option", "Actions" });

    editableOptionsTable->horizontalHeader()->setStyleSheet(QString(
        "QHeaderView::section {"
        "  background-color: #F0F0F0;"
        "  color: #333333;"
        "  padding: 12px 8px;"
        "  border: 1px solid #DDDDDD;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "}"
        ));

    editableOptionsTable->verticalHeader()->setVisible(false);
    editableOptionsTable->setSelectionMode(QAbstractItemView::NoSelection);
    editableOptionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    editableOptionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    editableOptionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    editableOptionsTable->setStyleSheet(QString(R"(
        QTableWidget {
            background-color: #FFFFFF;
            color: #333333;
            border: 1px solid #DDDDDD;
            border-radius: 8px;
            font-size: 15px;
            gridline-color: #EEEEEE;
        }
        QTableWidget::item {
            padding: 12px 8px;
            border-bottom: 1px solid #F0F0F0;
        }
        QTableWidget::item:selected {
            background-color: #E3F2FD;
            color: #333333;
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

    editableOptionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    editableOptionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    editableOptionsTable->setColumnWidth(1, 100);

    customizeOptionsLayout->addWidget(editableOptionsTable);

    // Action buttons
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    buttonLayout->addStretch(1);

    cancelCustomizeOptionsButton = new QPushButton("Cancel", customizeOptionsView);
    cancelCustomizeOptionsButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #E74C3C;"
        "  padding: 12px 20px;"
        "  border: 1px solid #E74C3C;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FADBD8;"
        "  color: #C0392B;"
        "  border-color: #C0392B;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #E74C3C;"
        "  color: white;"
        "}"
        ));
    cancelCustomizeOptionsButton->setCursor(Qt::PointingHandCursor);
    connect(cancelCustomizeOptionsButton, &QPushButton::clicked, this, &MyPollsPage::onCancelCustomizeOptionsClicked);
    buttonLayout->addWidget(cancelCustomizeOptionsButton);

    saveOptionsButton = new QPushButton("Save Changes", customizeOptionsView);
    saveOptionsButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: #3498DB;"
        "  color: white;"
        "  padding: 12px 24px;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980B9;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1C6EA4;"
        "}"
        ));
    saveOptionsButton->setCursor(Qt::PointingHandCursor);
    connect(saveOptionsButton, &QPushButton::clicked, this, &MyPollsPage::onSaveOptionsClicked);
    buttonLayout->addWidget(saveOptionsButton);

    customizeOptionsLayout->addLayout(buttonLayout);
    customizeOptionsLayout->addStretch();

    contentStack->addWidget(customizeOptionsView);

    // Connect add option button
    connect(addOptionButton, &QPushButton::clicked, this, [this, newOptionInput]()
            {
                QString text = newOptionInput->text().trimmed();
                if (!text.isEmpty())
                {
                    int row = editableOptionsTable->rowCount();
                    editableOptionsTable->insertRow(row);

                    QTableWidgetItem* textItem = new QTableWidgetItem(text);
                    textItem->setFlags(textItem->flags() | Qt::ItemIsEditable);
                    editableOptionsTable->setItem(row, 0, textItem);

                    QPushButton* deleteBtn = new QPushButton(editableOptionsTable);
                    deleteBtn->setText("Delete");
                    deleteBtn->setStyleSheet(QString(
                        "QPushButton { background-color: transparent; color: #E74C3C; padding: 3px 6px; border: 1px solid #E74C3C; border-radius: 4px; font-size: 12px; }"
                        "QPushButton:hover { background-color: #FADBD8; color: #C0392B; border-color: #C0392B; }"
                        "QPushButton:pressed { background-color: #E74C3C; color: white; }"
                        ));
                    deleteBtn->setCursor(Qt::PointingHandCursor);
                    editableOptionsTable->setCellWidget(row, 1, deleteBtn);

                    connect(deleteBtn, &QPushButton::clicked, this, [this, row]()
                            {
                                editableOptionsTable->removeRow(row);
                            });

                    editableOptionsTable->resizeRowsToContents();
                    editableOptionsTable->scrollToItem(textItem);
                    textItem->setBackground(QColor(232, 245, 233)); // Light green

                    newOptionInput->clear();
                    newOptionInput->setFocus();
                }
            });

    // Allow Enter key to add new option
    connect(newOptionInput, &QLineEdit::returnPressed, addOptionButton, &QPushButton::click);
}

void MyPollsPage::setupVotersView()
{
    votersPage = new QWidget(contentStack);
    QVBoxLayout* votersLayout = new QVBoxLayout(votersPage);

    votersTree = new QTreeWidget(votersPage);
    votersTree->setHeaderLabels({ "Option", "Name", "Address", "Phone" });
    votersTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    votersTree->setStyleSheet(R"(
        QTreeWidget {
            font-size: 16px;
            color: black;
            background-color: #FFFFFF;
            border: 1px solid #DDDDDD;
            border-radius: 6px;
            outline: 0px;
        }
        QTreeWidget::item {
            padding-top: 6px;
            padding-bottom: 6px;
            height: 40px;
        }
        QHeaderView::section {
            background-color: #F0F0F0;
            color: black;
            font-size: 16px;
            padding: 8px;
            border: 1px solid #DDDDDD;
            font-weight: bold;
        }
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
    )");

    votersTree->setSelectionMode(QAbstractItemView::NoSelection);
    votersTree->header()->setSectionResizeMode(QHeaderView::Stretch);

    QScrollArea* scrollArea = new QScrollArea(votersPage);
    scrollArea->setWidget(votersTree);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    votersLayout->addWidget(scrollArea);

    QPushButton* backButton = new QPushButton("Back", votersPage);
    backButton->setStyleSheet(QString(
        "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #2980B9; }"
        "QPushButton:pressed { background-color: #0056b3; }"
        ));
    backButton->setCursor(Qt::PointingHandCursor);
    votersLayout->addWidget(backButton);

    votersPage->setLayout(votersLayout);

    connect(backButton, &QPushButton::clicked, this, [this]()
            {
                contentStack->setCurrentWidget(pollListView);
                votersTree->clear();
            });

    connect(votersTree, &QTreeWidget::itemClicked, this, [](QTreeWidgetItem* item)
            {
                if (item && !item->parent())
                {
                    item->setExpanded(!item->isExpanded());
                }
            });

    contentStack->addWidget(votersPage);
}

void MyPollsPage::displayEditDescription()
{
    auto curPoll = retrieve_public_poll(currentPollId_);
    editDescriptionTitleLabel->setText(QString("Edit Description for %1").arg(curPoll.name));
    descriptionTextEdit->setText(QString::fromStdString(curPoll.desc));
    descriptionTextEdit->setFocus();
}

void MyPollsPage::displayCustomizeOptions()
{
    auto curPoll = retrieve_public_poll(currentPollId_);
    customizeOptionsTitleLabel->setText(QString("Customize Options for \"%1\"").arg(QString::fromStdString(curPoll.name)));

    auto poll = retrieve_public_poll(currentPollId_);

    editableOptionsTable->clearContents();
    editableOptionsTable->setRowCount(poll.options.size());

    for (size_t i = 0; i < poll.options.size(); i++)
    {
        const auto& option = poll.options[i];
        QTableWidgetItem* textItem = new QTableWidgetItem(QString::fromStdString(option.name));
        textItem->setFlags(textItem->flags() | Qt::ItemIsEditable);
        editableOptionsTable->setItem(i, 0, textItem);

        QPushButton* deleteBtn = new QPushButton("Delete");
        deleteBtn->setStyleSheet(QString(
            "QPushButton { background-color: transparent; color: #E74C3C; padding: 3px 6px; border: 1px solid #E74C3C; border-radius: 4px; font-size: 12px; }"
            "QPushButton:hover { background-color: #FADBD8; color: #C0392B; border-color: #C0392B; }"
            "QPushButton:pressed { background-color: #E74C3C; color: white; }"
            ));
        deleteBtn->setCursor(Qt::PointingHandCursor);
        editableOptionsTable->setCellWidget(i, 1, deleteBtn);

        connect(deleteBtn, &QPushButton::clicked, this, [this, deleteBtn]() {
            int clickedRow = -1;
            for (int row = 0; row < editableOptionsTable->rowCount(); ++row) {
                if (editableOptionsTable->cellWidget(row, 1) == deleteBtn) {
                    clickedRow = row;
                    break;
                }
            }
            if (clickedRow != -1) {
                editableOptionsTable->removeRow(clickedRow);
                editableOptionsTable->resizeRowsToContents();
            }
        });
    }

    editableOptionsTable->resizeColumnsToContents();
    editableOptionsTable->resizeRowsToContents();
    editableOptionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    editableOptionsTable->setColumnWidth(1, 80);
}

void MyPollsPage::onEditDescriptionClicked(size_t pollId)
{
    auto pollDetails = retrieve_poll_results(pollId);
    if (pollDetails.pollInfo.id == pollId)
    {
        currentPollId_ = pollId;
        displayEditDescription();
        contentStack->setCurrentWidget(editDescriptionView);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not retrieve poll details for editing description.");
    }
}

void MyPollsPage::onCustomizeOptionsClicked(size_t pollId)
{
    auto pollDetails = retrieve_poll_results(pollId);
    if (pollDetails.pollInfo.id == pollId)
    {
        bool hasVotes = false;
        for (const auto& option : pollDetails.results)
        {
            if (option.option_votes_count > 0)
            {
                hasVotes = true;
                break;
            }
        }

        if (pollDetails.pollInfo.is_finished)
        {
            QMessageBox::warning(this, "Cannot Customize", "This poll is already finished and cannot be customized.");
        }
        else if (hasVotes)
        {
            QMessageBox::warning(this, "Cannot Customize", "This poll has already received votes and its options cannot be customized.");
        }
        else
        {
            currentPollId_ = pollId;
            displayCustomizeOptions();
            contentStack->setCurrentWidget(customizeOptionsView);
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not retrieve poll details for customizing options.");
    }
}

void MyPollsPage::onSaveDescriptionClicked()
{
    auto newDescription = descriptionTextEdit->text().toStdString();

    if (newDescription.find('`') != std::string::npos)
    {
        QMessageBox::warning(nullptr, "Warning", "Invalid character detected!");
        return;
    }

    for (auto& poll : polls)
    {
        if (poll.id == currentPollId_)
        {
            poll.desc = newDescription;
        }
    }

    QMessageBox::information(this, "Success", "Poll description updated.");
    populatePollList();
    onBackToListClicked();
}

void MyPollsPage::onSaveOptionsClicked()
{
    MeshVector<std::string> updatedOptions;

    for (int row = 0; row < editableOptionsTable->rowCount(); row++)
    {
        QTableWidgetItem* item = editableOptionsTable->item(row, 0);
        if (item && !item->text().isEmpty())
        {
            updatedOptions.push_back(item->text().toStdString());
        }
    }

    if (updatedOptions.size() < 2)
    {
        QMessageBox::warning(this, "Invalid Options", "A poll must have at least two options.");
        return;
    }

    change_poll_options(currentPollId_, updatedOptions);

    QMessageBox::information(this, "Success", "Poll options updated.");
    populatePollList();
    onBackToListClicked();
}

void MyPollsPage::onCancelEditDescriptionClicked()
{
    onBackToListClicked(); // Go back without saving
}

void MyPollsPage::onCancelCustomizeOptionsClicked()
{
    onBackToListClicked(); // Go back without saving
}

void MyPollsPage::onSeeVotersClicked(size_t pollId)
{
    auto votersData = retrieve_poll_voters(pollId);

    if (!votersData.size())
    {
        QMessageBox::information(this, "No Voters", "No votes have been cast for this poll yet.");
        return;
    }

    votersTree->clear();

    for (auto& [optionName, voters] : votersData)
    {
        QTreeWidgetItem* optionItem = new QTreeWidgetItem(votersTree);
        optionItem->setText(0, QString::fromStdString(optionName));
        optionItem->setExpanded(true);

        for (auto& voter : voters)
        {
            QTreeWidgetItem* voterItem = new QTreeWidgetItem(optionItem);
            voterItem->setText(1, QString::fromStdString(voter.name));
            voterItem->setText(2, QString::fromStdString(voter.address));
            voterItem->setText(3, QString::fromStdString(voter.phone_number));
        }
    }

    contentStack->setCurrentWidget(votersPage);
}
