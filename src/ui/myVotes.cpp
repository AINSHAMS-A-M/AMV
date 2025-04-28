// helppage.cpp
#include "myVotes.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPushButton>
#include "utils.h"

MyVotesPage::MyVotesPage(QWidget *parent)
    : QWidget(parent),
    sidebar(nullptr),
    content(nullptr)
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
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onHelpClicked);
        } else if (item == "Vote") {
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onVoteClicked);
        } else if (item == "My Votes") {
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onMyVotesClicked);
        } else if (item == "Create Poll") {
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onCreatePollClicked);
        } else if (item == "My Polls") {
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onMyPollsClicked);
        } else if (item == "Profile") {
            connect(btn, &QPushButton::clicked, this, &::MyVotesPage::onProfileClicked);
        }
    }
    sbLayout->addStretch();

    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
    rootLayout->setStretch(1, 1);
}
