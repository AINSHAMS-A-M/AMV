#include "vote.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include "db.hpp"
#include "mainwindow.h"
#include "services.hpp"

VotePage::VotePage(QWidget *parent)
    : QWidget(parent),
    stackedWidget(nullptr),
    sidebar(nullptr),
    content(nullptr),
    voterIdPage(nullptr),
    pollOptionsPage(nullptr)
{
    const QString bgColor       = "#F5F6F8";
    const QString primaryColor  = "#007BFF";
    const QString borderColor   = "#D1D1D1";
    const QString primaryBtnColor = "#3498DB";
    const QString primaryBtnHoverColor = "#2980B9";
    const QString primaryBtnPressedColor = "#1F618D";
    const QString scrollbarBgColor = "#ECF0F1";
    const QString scrollbarHandleColor = "#BDC3C7";
    const QString scrollbarHandleHoverColor = "#95A5A6";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Create sidebar
    sidebar = new SidebarWidget(this,"Vote");

    // Create content area
    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // Create stacked widget for voter ID and poll options pages
    stackedWidget = new QStackedWidget(content);

    // --- Create voter ID page ---

    voterIdPage = new QWidget(stackedWidget);
    auto *voterIdLayout = new QVBoxLayout(voterIdPage);
    voterIdLayout->setContentsMargins(50, 50, 50, 50);
    voterIdLayout->setSpacing(25);
    voterIdLayout->setAlignment(Qt::AlignCenter);
    voterIdLayout->addStretch(1);

    QLabel *voterTitle = new QLabel("Vote in Poll", voterIdPage);
    voterTitle->setStyleSheet("font-size: 50px; font-weight: bold; color: #333; margin-bottom: 15px;"); // Increased font size and added margin
    voterIdLayout->addWidget(voterTitle, 0, Qt::AlignHCenter);

    // Create a form for voter ID
    QWidget *formWidget = new QWidget(voterIdPage);
    formWidget->setStyleSheet(QString(
                                  "QLabel { font-size: 16px; color: #333; margin-bottom: 5px; }" // Added margin-bottom to label
                                  "QLineEdit { font-size: 16px; padding: 12px; border: 1px solid %1; border-radius: 8px; min-width: 400px; }" // Increased padding and min-width
                                  "QLineEdit:focus { border: 2px solid %2; }"
                                  ).arg(borderColor, primaryColor));

    auto *formLayout = new QVBoxLayout(formWidget);
    formLayout->setSpacing(10);
    formLayout->setAlignment(Qt::AlignCenter);

    // Voter ID input
    QLabel *voterIdLabel = new QLabel("Enter your voter ID (Get it from the poll's owner):", formWidget);
    voterIdInput = new QLineEdit(formWidget);
    voterIdInput->setPlaceholderText("Voter ID");
    voterIdInput->setObjectName("voterInput");
    voterIdInput->setAlignment(Qt::AlignCenter);
    connect(voterIdInput, &QLineEdit::returnPressed, this, &VotePage::validateVoterCredentials);

    formLayout->addWidget(voterIdLabel, 0, Qt::AlignCenter);
    formLayout->addWidget(voterIdInput, 0, Qt::AlignCenter);

    // Add form widget centered
    voterIdLayout->addWidget(formWidget, 0, Qt::AlignHCenter);

    voterIdLayout->addSpacing(20);

    // Add submit button
    QPushButton *submitBtn = new QPushButton("Vote", voterIdPage);
    submitBtn->setStyleSheet(QString(
                                 "QPushButton { background-color: %1; color: white; font-size: 18px; padding: 12px 40px; border-radius: 8px; min-width: 150px; }" // Increased padding
                                 "QPushButton:hover { background-color: %2; }"
                                 "QPushButton:pressed { background-color: %3; }"
                                 ).arg(primaryBtnColor,primaryBtnHoverColor,primaryBtnPressedColor));
    submitBtn->setCursor(Qt::PointingHandCursor);
    connect(submitBtn, &QPushButton::clicked, this, &VotePage::validateVoterCredentials);
    voterIdLayout->addWidget(submitBtn, 0, Qt::AlignHCenter);

    voterIdLayout->addStretch(1);

    // --- Create poll options page ---
    pollOptionsPage = new QWidget(stackedWidget);
    auto *pollOptionsLayout = new QVBoxLayout(pollOptionsPage);
    pollOptionsLayout->setContentsMargins(20, 5, 20, 20);
    pollOptionsLayout->setSpacing(5);

    pollTitleLabel = new QLabel("Poll Title", pollOptionsPage);
    pollTitleLabel->setWordWrap(true);
    pollTitleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; background: transparent;");
    pollTitleLabel->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Fixed);
    pollOptionsLayout->addWidget(pollTitleLabel, 1, Qt::AlignLeft);

    pollDescLabel = new QLabel("Poll Description", pollOptionsPage);
    pollDescLabel->setWordWrap(true);
    pollDescLabel->setAlignment(Qt::AlignLeft);
    pollDescLabel->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Fixed);
    pollDescLabel->setStyleSheet(
        "font-size: 16px; color: #555; "
        );
    pollOptionsLayout->addWidget(pollDescLabel, 0);
    pollOptionsLayout->addSpacing(5);

    // Scroll area for poll options
    QScrollArea *scrollArea = new QScrollArea(pollOptionsPage);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(QString(
                                  "QScrollArea { border: none; background-color: transparent; }"
                                  "QScrollBar:vertical { background: %1; width: 12px; }"
                                  "QScrollBar::handle:vertical { background: %2; min-height: 25px; border-radius: 6px; }"
                                  "QScrollBar::handle:vertical:hover { background: %3; }"
                                  "QScrollBar::add-line, QScrollBar::sub-line { height: 0; }"
                                  "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"
                                  ).arg(scrollbarBgColor, scrollbarHandleColor, scrollbarHandleHoverColor));

    optionsContainer = new QWidget(scrollArea);
    optionsLayout = new QVBoxLayout(optionsContainer);
    optionsLayout->setContentsMargins(15, 15, 15, 15);
    optionsLayout->setSpacing(10);

    scrollArea->setWidget(optionsContainer);
    pollOptionsLayout->addWidget(scrollArea, 3);

    // Create button group for radio buttons
    optionsGroup = new QButtonGroup(pollOptionsPage);
    optionsGroup->setExclusive(true);

    // Add vertical spacing before buttons
    pollOptionsLayout->addSpacing(25);

    auto *btnRow = new QHBoxLayout();
    btnRow->setContentsMargins(0,0,0,0);
    btnRow->setSpacing(20);

    // Back on left
    QPushButton *backBtn = new QPushButton("Back", pollOptionsPage);
    backBtn->setStyleSheet(
        "QPushButton { background-color: #6C757D; color: white;"
        "font-size: 14px; padding: 8px 20px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #5A6268; }"
        );
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, &VotePage::showVoterIdPage);
    btnRow->addWidget(backBtn);

    // spacer pushes submit to the right
    btnRow->addStretch(1);

    // Submit on right
    QPushButton *voteSubmitBtn = new QPushButton("Submit Vote", pollOptionsPage);
    voteSubmitBtn->setStyleSheet(QString(
                                     "QPushButton { background-color: %1; color: white;"
                                     "font-size: 16px; padding: 12px 30px; border-radius: 8px; }"
                                     "QPushButton:hover { background-color: %2; }"
                                     "QPushButton:pressed { background-color: %3; }"
                                     ).arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor));
    voteSubmitBtn->setCursor(Qt::PointingHandCursor);
    connect(voteSubmitBtn, &QPushButton::clicked, this, &VotePage::submitVote);
    btnRow->addWidget(voteSubmitBtn);

    // add the row to your main layout
    pollOptionsLayout->addLayout(btnRow);

    // Add both pages to stacked widget
    stackedWidget->addWidget(voterIdPage);
    stackedWidget->addWidget(pollOptionsPage);

    // Default to showing voter ID page
    stackedWidget->setCurrentWidget(voterIdPage);

    contentLayout->addWidget(stackedWidget);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
    rootLayout->setStretch(1, 1);

}

void VotePage::validateVoterCredentials()
{
    std::string voterIdStr = voterIdInput->text().toStdString();
    if (voterIdStr.empty()) {
        QMessageBox::warning(this, "Input Required", "Please enter your Voter ID.");
        voterIdInput->setFocus();
        return;
    }

    std::string pollIdToLoad = getPollId(voterIdStr);
    if (pollIdToLoad == "f")
    {
        QMessageBox::warning(this, "Invalid Voter ID", "The entered Voter ID does not exist.");
        voterIdInput->clear();
        voterIdInput->setFocus();
        return;
    }

    if (check_user_vote(activeUser.id,(size_t)stoll(pollIdToLoad)).first) {
        QMessageBox::information(this, "Already Voted", "You have already cast your vote in this poll.");
        voterIdInput->clear();
        stackedWidget->setCurrentWidget(voterIdPage);
        voterIdInput->setFocus();
        return;
    }
    pollId = (size_t)stoll(pollIdToLoad);
    loadPollOptions();

    stackedWidget->setCurrentWidget(pollOptionsPage);
}

void VotePage::loadPollOptions()
{
    QLayoutItem *item;
    while ((item = optionsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            QAbstractButton* btn = qobject_cast<QAbstractButton*>(item->widget());
            if(btn && optionsGroup) {
                optionsGroup->removeButton(btn);
            }
            item->widget()->deleteLater();
        }
        delete item;
    }
    optionsLayout->addStretch();

    // Fetch poll details from database
    std::string pollTitle, pollDesc;
    MeshVector<std::string> options;

    // Call the function to get details
    auto pollData = retrieve_public_poll(pollId);
    pollTitle = pollData.name;
    pollDesc = pollData.desc;

    // Set poll title and description
    pollTitleLabel->setText(QString::fromStdString(pollTitle));
    pollDescLabel->setText(QString::fromStdString(pollDesc));

    // Get poll options
    for (auto &option : pollData.options)
    {
        options.push_back(option.name);
    }

    // Remove the stretch temporarily before adding options
    if(optionsLayout->count() > 0) {
        QLayoutItem* stretchItem = optionsLayout->itemAt(optionsLayout->count() - 1);
        if (stretchItem->spacerItem()) {
            optionsLayout->removeItem(stretchItem);
            delete stretchItem;
        }
    }


    // Add options as radio buttons
    size_t optionId = getPollOptionId(pollId);
    for (const auto &option : options) {
        QRadioButton *radioBtn = new QRadioButton(QString::fromStdString(option), optionsContainer);
        radioBtn->setStyleSheet(
            "QRadioButton { font-size: 16px; padding: 12px 15px; background-color: white; border: 1px solid #DDD; "
            "border-radius: 8px; margin: 5px 0px; }"
            "QRadioButton:hover { background-color: #F0F0F0; border-color: #CCC; }"
            "QRadioButton::indicator { width: 0px; height: 0px; }"
            "QRadioButton:checked { background-color: #E1F5FE; border: 2px solid #007BFF; font-weight: bold; }"
            );
        radioBtn->setCursor(Qt::PointingHandCursor);
        optionsGroup->addButton(radioBtn, optionId++);
        optionsLayout->addWidget(radioBtn);
    }

    optionsLayout->addStretch();
}

void VotePage::submitVote()
{
    QAbstractButton *selectedOption = optionsGroup->checkedButton();
    if (!selectedOption) {
        QMessageBox::warning(this, "No Selection", "Please select an option before submitting your vote.");
        return;
    }

    size_t selectedOptionId = optionsGroup->id(selectedOption);

    create_user_vote(activeUser.id,pollId,selectedOptionId);
    QMessageBox::information(this, "Success", "Your vote has been recorded successfully!");
    // Reset and go back to voter ID page
    voterIdInput->clear();
    showVoterIdPage(); // Go back to the start page

}

void VotePage::showVoterIdPage()
{

    optionsGroup->setExclusive(false);
    QAbstractButton* btn = optionsGroup->checkedButton();
    if (btn) {
        btn->setChecked(false);
    }
    optionsGroup->setExclusive(true);


    pollTitleLabel->setText("Poll Title");
    pollDescLabel->setText("Poll Description");

    // Switch page
    stackedWidget->setCurrentWidget(voterIdPage);
    voterIdInput->setFocus();
    voterIdInput->clear();
    voterIdInput->selectAll();
}
