// helppage.cpp
#include "help.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPushButton>
#include "db.hpp"
#include "utils.h"

HelpPage::HelpPage(QWidget *parent)
    : QWidget(parent),
    sidebar(nullptr),
    content(nullptr),
    titleLabel(nullptr),
    helpTextEdit(nullptr)
{
    const QString sidebarColor  = "#2C3E50";
    const QString bgColor       = "#F5F6F8";
    const QString primaryColor  = "#007BFF";
    const QString primaryHover  = "#339CFF";
    const QString textColor     = "#FFFFFF";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new QWidget(this);
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet(QString("background-color: %1;").arg(sidebarColor));
    auto *sbLayout = new QVBoxLayout(sidebar);
    sbLayout->setSpacing(15);
    sbLayout->setContentsMargins(20, 20, 20, 20);

    const int logoSize = 120;
    QPixmap logoPixmap(":/logo.png");

    QPixmap roundedLogo = createRoundedLogo(logoPixmap, logoSize);
    if (!roundedLogo.isNull()) {
        QLabel *logoLabel = new QLabel(sidebar);
        logoLabel->setPixmap(roundedLogo);
        logoLabel->setFixedSize(logoSize, logoSize);
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setStyleSheet("background-color: transparent;");
        sbLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);
    } else {
        QLabel *errorLabel = new QLabel("Logo Error", sidebar);
        errorLabel->setAlignment(Qt::AlignCenter);
        errorLabel->setStyleSheet(QString("color: %1;").arg(textColor));
        sbLayout->addWidget(errorLabel, 0, Qt::AlignHCenter);
    }

    QStringList menuItems = {"Help", "Vote", "My Votes", "Create Poll", "My Polls", "Profile"};
    for (const QString &item : menuItems) {
        QPushButton *btn = new QPushButton(item, sidebar);
        btn->setStyleSheet(QString(
                               "QPushButton { color: %1; background: none; border: none; text-align: left; font-size: 18px; padding: 10px; }"
                               "QPushButton:hover { background-color: rgba(255,255,255,0.1); }"
                               ).arg(textColor));
        btn->setCursor(Qt::PointingHandCursor);
        sbLayout->addWidget(btn);
        if (item == "Help") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onHelpClicked);
        } else if (item == "Vote") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onVoteClicked);
        } else if (item == "My Votes") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onMyVotesClicked);
        } else if (item == "Create Poll") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onCreatePollClicked);
        } else if (item == "My Polls") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onMyPollsClicked);
        } else if (item == "Profile") {
            connect(btn, &QPushButton::clicked, this, &::HelpPage::onProfileClicked);
        }
    }
    QLabel *welcomeLabel = new QLabel("Welcome \n" + QString::fromStdString(activeUser.name) + "!", sidebar);
    welcomeLabel->setStyleSheet(QString("color: %1; font-size: 15px; padding: 5px 10px; font-style: italic; font-weight: bold;").arg("#EBECF0"));
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setAlignment(Qt::AlignLeft);
    sbLayout->addWidget(welcomeLabel);
    sbLayout->addStretch();

    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    titleLabel = new QLabel("Application Help", content);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; padding: 20px;");
    contentLayout->addWidget(titleLabel);

    helpTextEdit = new QTextEdit(content);
    helpTextEdit->setReadOnly(true);
    helpTextEdit->setStyleSheet(
        "QTextEdit {"
        "  background-color: #FFFFFF;"
        "  border: none;"
        "  font-size: 14px;"
        "  color: #333;"
        "  padding-left: 35px;"
        "  padding-top: 15px;"
        "}"
        );
    helpTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QString helpContent =
        "<h3>Help</h3>"
        "<p>This page provides information about the different sections and functionalities of the application.</p>"
        "<h3>Vote</h3>"
        "<p>Allows you to participate in a new poll. You will typically need a valid Voter ID to cast your vote.</p>"

        "<h3>My Votes</h3>"
        "<p>View a history of the polls you have participated in and the options you selected. This section might also allow you to remove or change a previous vote.</p>"

        "<h3>Create Poll</h3>"
        "<p>Provides tools for creating a new election or poll. You can define the poll's name, and add the various options that users can vote for.</p>"

        "<h3>My Polls</h3>"
        "<p>View the polls you have created, and their results thus far.</p>"

        "<h3>Profile</h3>"
        "<p>Edit your user account information, including your real name, username, and password. Keep your profile details up-to-date here.</p>";

    helpTextEdit->setHtml(helpContent);
    contentLayout->addWidget(helpTextEdit);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
}
