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
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QInputDialog>
#include <iostream>
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
    optionsTable(nullptr),
    editDescriptionView(nullptr),
    customizeOptionsView(nullptr),
    editDescriptionLayout(nullptr),
    editDescriptionTitleLabel(nullptr),
    descriptionTextEdit(nullptr),
    saveDescriptionButton(nullptr),
    cancelEditDescriptionButton(nullptr),
    customizeOptionsLayout(nullptr),
    customizeOptionsTitleLabel(nullptr),
    editableOptionsTable(nullptr),
    addOptionButton(nullptr),
    saveOptionsButton(nullptr),
    cancelCustomizeOptionsButton(nullptr)
{
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
    setupEditDescriptionView();
    setupCustomizeOptionsView();

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

void MyPollsPage::setupPollDetailsView() {
    pollDetailsView = new QWidget(contentStack);
    pollDetailsLayout = new QVBoxLayout(pollDetailsView);
    pollDetailsLayout->setContentsMargins(0, 0, 0, 0);
    pollDetailsLayout->setSpacing(15);

    // Header layout for back button and title
    auto *headerLayout = new QHBoxLayout();
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
            auto poll = retrieve_poll_results(activeUser.id, id);

            // Create a widget for the poll card
            QWidget *cardWidget = new QWidget(pollListContainerWidget);
            cardWidget->setObjectName("pollCard");
            // Border is green for ongoing, light gray for finished
            bool isDone = poll.pollInfo.is_finished;
            cardWidget->setStyleSheet(QString(
                                          "QWidget {"
                                          "  background-color: #FFFFFF;"
                                          "  border-radius: 8px;}"
                                          "#pollCard {"
                                          "  background-color: #FFFFFF;"
                                          "  border-radius: 8px;"
                                          "  border: %1;"
                                          "}"
                                          ).arg(isDone
                                                   ? "1px solid #DDDDDD"
                                                   : "2px solid #27AE60"
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

            // Small text-only badge
            QLabel *stateBadge = new QLabel(
                isDone ? "Finished" : "Ongoing",
                cardWidget
                );
            stateBadge->setStyleSheet(QString(
                                          "font-size: 12px;"
                                          "font-weight: bold;"
                                          "color: %1;"
                                          ).arg(isDone
                                                   ? "#7F8C8D"
                                                   : "#27AE60"
                                               ));
            contentLayout->addWidget(stateBadge);

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

            QPushButton *endElectionButton = new QPushButton("End Election", cardWidget);
            endElectionButton->setStyleSheet(QString(
                                                 "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                                                 "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                                                 "QPushButton:pressed { background-color: %1; color: white; }"
                                                 ).arg(dangerColor, dangerHoverBg, dangerHoverFg));
            endElectionButton->setCursor(Qt::PointingHandCursor);
            endElectionButton->setFixedWidth(120);

            // add it to the same buttonsLayout
            buttonsLayout->addWidget(endElectionButton);

            // connect its click to a new slot
            connect(endElectionButton, &QPushButton::clicked, this, [ this, id]() {
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

            // Delete button
            QPushButton *deleteButton = new QPushButton("Delete", cardWidget);
            deleteButton->setStyleSheet(QString(
                                            "QPushButton { background-color: transparent; color: %1; padding: 8px 12px; border: 1px solid %1; border-radius: 8px; font-size: 13px; }"
                                            "QPushButton:hover { background-color: %2; color: %3; border-color: %1; }"
                                            "QPushButton:pressed { background-color: %1; color: white; }"
                                            ).arg(dangerColor, dangerHoverBg, dangerHoverFg));
            deleteButton->setCursor(Qt::PointingHandCursor);
            deleteButton->setFixedWidth(100);
            buttonsLayout->addWidget(deleteButton);

            // Edit Description Button
            QPushButton *editDescriptionButton = new QPushButton("Edit Description", cardWidget);
            editDescriptionButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
                ));
            editDescriptionButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(editDescriptionButton);
            connect(editDescriptionButton, &QPushButton::clicked, this, [this, id]() { onEditDescriptionClicked(id); });

            // Customize options
            QPushButton *customizeOptionsButton = new QPushButton("Customize Options", cardWidget);
            customizeOptionsButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
                ));
            customizeOptionsButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(customizeOptionsButton);
            connect(customizeOptionsButton, &QPushButton::clicked, this, [this, id]() { onCustomizeOptionsClicked(id); });


            // Add "See Voters" button
            QPushButton *seeVotersButton = new QPushButton("See Voters", cardWidget);
            seeVotersButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
                ));
            seeVotersButton->setCursor(Qt::PointingHandCursor);
            buttonsLayout->addWidget(seeVotersButton);
            connect(seeVotersButton, &QPushButton::clicked, this, [this, id]() { onSeeVotersClicked(id); });


            // View button
            QPushButton *viewButton = new QPushButton("View", cardWidget);
            viewButton->setStyleSheet(QString(
                "QPushButton { background-color: #3498DB; color: white; padding: 9px 13px; border: none; border-radius: 8px; font-weight: bold; font-size: 13px; }"
                "QPushButton:hover { background-color: #2980B9; }"
                "QPushButton:pressed { background-color: #0056b3; }"
                ));
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
    auto pollDetails = retrieve_poll_results(activeUser.id, pollId);
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


void MyPollsPage::setupEditDescriptionView() {
    editDescriptionView = new QWidget(contentStack);
    editDescriptionLayout = new QVBoxLayout(editDescriptionView);
    editDescriptionLayout->setContentsMargins(0, 250, 0, 0);
    editDescriptionLayout->setSpacing(50);
    editDescriptionLayout->setAlignment(Qt::AlignCenter);
    // editDescriptionLayout->addStretch(1);

    editDescriptionTitleLabel = new QLabel("Edit Poll Description", editDescriptionView);
    editDescriptionTitleLabel->setAlignment(Qt::AlignCenter);
    editDescriptionTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    editDescriptionLayout->addWidget(editDescriptionTitleLabel);

    descriptionTextEdit = new QLineEdit(editDescriptionView);
    descriptionTextEdit->setPlaceholderText("Enter poll description...");
    descriptionTextEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}"
    );

    descriptionTextEdit->setMinimumHeight(50);

    editDescriptionLayout->addWidget(descriptionTextEdit);

    auto *buttonLayout = new QHBoxLayout();

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




void MyPollsPage::setupCustomizeOptionsView() {
    customizeOptionsView = new QWidget(contentStack);
    customizeOptionsLayout = new QVBoxLayout(customizeOptionsView);
    customizeOptionsLayout->setContentsMargins(0, 100, 0, 50);
    customizeOptionsLayout->setSpacing(15);

    customizeOptionsTitleLabel = new QLabel("Customize Poll Options", customizeOptionsView);
    customizeOptionsTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333333;");
    customizeOptionsLayout->addWidget(customizeOptionsTitleLabel);

    // Description text
    QLabel *descriptionLabel = new QLabel("Add, edit, or remove options for your poll below.", customizeOptionsView);
    descriptionLabel->setStyleSheet("font-size: 16px; color: #666666; margin-bottom: 10px;");
    customizeOptionsLayout->addWidget(descriptionLabel);

    // Create inline option input area with modern styling
    QWidget *addOptionWidget = new QWidget(customizeOptionsView);
    addOptionWidget->setObjectName("addOptionWidget");
    addOptionWidget->setStyleSheet(
        "#addOptionWidget {"
        "  padding: 0px;"  // Removed all background and border
        "  margin-bottom: 15px;"
        "}"
        );

    QHBoxLayout *addOptionLayout = new QHBoxLayout(addOptionWidget);
    addOptionLayout->setContentsMargins(0, 0, 0, 0);  // Tighter margins
    addOptionLayout->setSpacing(12);

    // Input field for new option
    QLineEdit *newOptionInput = new QLineEdit(addOptionWidget);
    newOptionInput->setPlaceholderText("Enter new option text...");
    newOptionInput->setStyleSheet(
        "QLineEdit {"
        "  border: 1px solid #D1D5DB;"
        "  border-radius: 6px;"
        "  padding: 10px 12px;"
        "  font-size: 15px;"
        "  background-color: white;"
        "  margin: 0px;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #3498DB;"
        "  outline: none;"
        "}"
        );

    // Add option button with improved styling
    QPushButton *addOptionButton = new QPushButton("Add Option", addOptionWidget);
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


    // Add the option input widget to main layout
    customizeOptionsLayout->addWidget(addOptionWidget);

    // Table for existing options
    editableOptionsTable = new QTableWidget(customizeOptionsView);
    editableOptionsTable->setColumnCount(2); // Option Text, Actions (Delete)
    editableOptionsTable->setHorizontalHeaderLabels({"Option Text", "Actions"});
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
    editableOptionsTable->setColumnWidth(1, 100); // Width for delete button column

    customizeOptionsLayout->addWidget(editableOptionsTable);

    // Action buttons layout
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    buttonLayout->addStretch(1); // Push buttons to the right

    // Cancel button
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

    // Save button
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
    customizeOptionsLayout->addStretch(); // Push content to the top

    contentStack->addWidget(customizeOptionsView);

    // Connect add option button to add new row
    connect(addOptionButton, &QPushButton::clicked, this, [this, newOptionInput]() {
        QString text = newOptionInput->text().trimmed();
        if (!text.isEmpty()) {
            int row = editableOptionsTable->rowCount();
            editableOptionsTable->insertRow(row);

            // Create styled item for the option text
            QTableWidgetItem *textItem = new QTableWidgetItem(text);
            textItem->setFlags(textItem->flags() | Qt::ItemIsEditable);
            editableOptionsTable->setItem(row, 0, textItem);

            // Create styled delete button for the row
            QPushButton *deleteBtn = new QPushButton(editableOptionsTable);
            deleteBtn->setText("Delete");
            deleteBtn->setStyleSheet(QString(
                "QPushButton { background-color: transparent; color: #E74C3C; padding: 3px 6px; border: 1px solid #E74C3C; border-radius: 4px; font-size: 12px; }"
                "QPushButton:hover { background-color: #FADBD8; color: #C0392B; border-color: #C0392B; }"
                "QPushButton:pressed { background-color: #E74C3C; color: white; }"
                ));
            deleteBtn->setCursor(Qt::PointingHandCursor);
            editableOptionsTable->setCellWidget(row, 1, deleteBtn);

            // Connect delete button to remove the row
            connect(deleteBtn, &QPushButton::clicked, this, [this, row]() {
                editableOptionsTable->removeRow(row);
            });
            editableOptionsTable->resizeRowsToContents(); // Adjust row height

            // Clear input field after adding
            newOptionInput->clear();
            newOptionInput->setFocus();

            // Adjust row height and ensure the new row is visible
            editableOptionsTable->resizeRowsToContents();
            editableOptionsTable->scrollToItem(textItem);

            textItem->setBackground(QColor(232, 245, 233)); // Light green
        }
    });

    // Also allow adding option with Enter key
    connect(newOptionInput, &QLineEdit::returnPressed, addOptionButton, &QPushButton::click);
}




void MyPollsPage::displayEditDescription() {
    auto curPoll = retrieve_public_poll(currentPollId_);
    editDescriptionTitleLabel->setText(QString("Edit Description for %1").arg(curPoll.name));
    descriptionTextEdit->setText(QString::fromStdString(curPoll.desc));
    descriptionTextEdit->setFocus();
}

void MyPollsPage::displayCustomizeOptions() {
    auto curPoll = retrieve_public_poll(currentPollId_);
    customizeOptionsTitleLabel->setText(QString("Customize Options for \"%1\"").arg(QString::fromStdString(curPoll.name)));
    auto poll = retrieve_public_poll(currentPollId_);
    // Populate the editable options table
    editableOptionsTable->clearContents();
    editableOptionsTable->setRowCount(poll.options.size()); // Start with existing options
    for (size_t i = 0; i < poll.options.size(); i++) {
        const auto& option = poll.options[i];
        QTableWidgetItem *textItem = new QTableWidgetItem(QString::fromStdString(option.name));
        textItem->setFlags(textItem->flags() | Qt::ItemIsEditable); // Make option text editable here
        editableOptionsTable->setItem(i, 0, textItem);

        // Add a delete button for each existing row
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setStyleSheet(QString(
            "QPushButton { background-color: transparent; color: #E74C3C; padding: 3px 6px; border: 1px solid #E74C3C; border-radius: 4px; font-size: 12px; }"
            "QPushButton:hover { background-color: #FADBD8; color: #C0392B; border-color: #C0392B; }"
            "QPushButton:pressed { background-color: #E74C3C; color: white; }"
            ));
        deleteBtn->setCursor(Qt::PointingHandCursor);
        editableOptionsTable->setCellWidget(i, 1, deleteBtn);

        // Connect delete button signal
        connect(deleteBtn, &QPushButton::clicked, this, [this, i]() {
            editableOptionsTable->removeRow(i);  // Use captured row index `i`
            editableOptionsTable->resizeRowsToContents();
        });
    }

    editableOptionsTable->resizeColumnsToContents();
    editableOptionsTable->resizeRowsToContents();
    editableOptionsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    editableOptionsTable->setColumnWidth(1, 80);
}



void MyPollsPage::onEditDescriptionClicked(size_t pollId) {

    auto pollDetails = retrieve_poll_results(activeUser.id, pollId);
    if (pollDetails.pollInfo.id == pollId) {
        currentPollId_ = pollId;
        displayEditDescription();
        contentStack->setCurrentWidget(editDescriptionView);
    } else {
        QMessageBox::warning(this, "Error", "Could not retrieve poll details for editing description.");
    }
}

void MyPollsPage::onCustomizeOptionsClicked(size_t pollId) {
    auto pollDetails = retrieve_poll_results(activeUser.id, pollId);
    if (pollDetails.pollInfo.id == pollId) {
        bool hasVotes = false;
        for(const auto& option : pollDetails.results) {
            if (option.option_votes_count > 0) {
                hasVotes = true;
                break;
            }
        }

        if (pollDetails.pollInfo.is_finished) {
            QMessageBox::warning(this, "Cannot Customize", "This poll is already finished and cannot be customized.");
        } else if (hasVotes) {
            QMessageBox::warning(this, "Cannot Customize", "This poll has already received votes and its options cannot be customized.");
        }
        else {
            currentPollId_ = pollId; // Store the ID
            displayCustomizeOptions(); // Populate the editor
            contentStack->setCurrentWidget(customizeOptionsView); // Switch to the edit view
        }
    } else {
        QMessageBox::warning(this, "Error", "Could not retrieve poll details for customizing options.");
    }
}

void MyPollsPage::onSaveDescriptionClicked() {

    auto newDescription = descriptionTextEdit->text().toStdString();
    if (newDescription.find('`') != std::string::npos)
    {
        QMessageBox::warning(nullptr,"Warning","Invalid character detected!");
        return;
    }
    for (auto &poll : polls)
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

void MyPollsPage::onSaveOptionsClicked() {
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

    change_poll_options(currentPollId_ , updatedOptions);
    QMessageBox::information(this, "Success", "Poll options updated.");
    populatePollList();
    onBackToListClicked();

}

void MyPollsPage::onCancelEditDescriptionClicked() {
    onBackToListClicked(); // Simply go back to the list without saving
}

void MyPollsPage::onCancelCustomizeOptionsClicked() {
    onBackToListClicked(); // Simply go back to the list without saving
}




void MyPollsPage::onSeeVotersClicked(size_t pollId) {
    // Fetch voter data (hypothetical function - must be implemented)
    auto votersData = retrieve_poll_voters(pollId); // Returns a structured list of voters per option

    if (!votersData.size()) {
        QMessageBox::information(this, "No Voters", "No votes have been cast for this poll yet.");
        return;
    }

    // Create a dialog to display voters
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Voter Details");
    dialog->resize(800, 600);

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    QTreeWidget *tree = new QTreeWidget(dialog);
    tree->setHeaderLabels({"Option", "Name", "Address", "Phone"});
    tree->header()->setSectionResizeMode(QHeaderView::Stretch);

    // Populate the tree with options and voters
    for (auto& [optionName, voters] : votersData)
    {
        QTreeWidgetItem *optionItem = new QTreeWidgetItem(tree);
        optionItem->setText(0, QString::fromStdString(optionName));
        optionItem->setExpanded(true);

        for (auto& voter : voters) {
            QTreeWidgetItem *voterItem = new QTreeWidgetItem(optionItem);
            voterItem->setText(1, QString::fromStdString(voter.name));
            voterItem->setText(2, QString::fromStdString(voter.address));
            voterItem->setText(3, QString::fromStdString(voter.phone_number));
        }
    }

    dialogLayout->addWidget(tree);

    // Close button
    QPushButton *closeButton = new QPushButton("Close", dialog);
    closeButton->setStyleSheet(QString(
        "QPushButton { background-color: #3498DB; color: white; padding: 10px 20px; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2980B9; }"
        ));
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    dialogLayout->addWidget(closeButton);

    dialog->setLayout(dialogLayout);
    dialog->exec();
}
