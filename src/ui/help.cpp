
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
#include "mainwindow.h"

HelpPage::HelpPage(QWidget *parent)
    : QWidget(parent),
    sidebar(nullptr),
    content(nullptr),
    titleLabel(nullptr),
    helpTextEdit(nullptr)
{
    const QString bgColor       = "#F5F6F8";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this,"Help");

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
        "<p>View a history of the polls you have participated in and the options you selected. This section might also allow you to remove a previous vote.</p>"

        "<h3>Create Poll</h3>"
        "<p>Provides tools for creating a new election or poll. You can define the poll's name, and add the various options that users can vote for.</p>"

        "<h3>My Polls</h3>"
        "<p>View the polls you have created, and their results thus far.</p>"

        "<h3>Edit Profile</h3>"
        "<p>Edit your user account information, including your real name, username, and password. Keep your profile details up-to-date here.</p>";

    helpTextEdit->setHtml(helpContent);
    contentLayout->addWidget(helpTextEdit);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);

}
