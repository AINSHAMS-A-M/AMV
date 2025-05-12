#include "help.h"
#include "mainwindow.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

HelpPage::HelpPage(QWidget* parent)
	: QWidget(parent),
	sidebar(nullptr),
	content(nullptr),
	titleLabel(nullptr),
	helpTextEdit(nullptr)
{
	const QString bgColor = "#F5F6F8";

	auto* rootLayout = new QHBoxLayout(this);
	rootLayout->setContentsMargins(0, 0, 0, 0);
	rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this, "Help and Credits");

	content = new QWidget(this);
	content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
	auto* contentLayout = new QVBoxLayout(content);
	contentLayout->setContentsMargins(0, 0, 0, 0);
	contentLayout->setSpacing(0);

    titleLabel = new QLabel("Help and Credits", content);
    titleLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: #333; padding: 20px;");
	contentLayout->addWidget(titleLabel);

	helpTextEdit = new QTextEdit(content);
	helpTextEdit->setReadOnly(true);
	helpTextEdit->setStyleSheet(
		"QTextEdit {"
		"  background-color: #FFFFFF;"
		"  border: none;"
        "  font-size: 16px;"
		"  color: #333;"
		"  padding-left: 35px;"
		"  padding-top: 15px;"
		"}"
	);
	helpTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	QString helpContent =
        "<h3>Vote</h3>"
        "<p>Allows you to participate in a new poll. You will typically need a valid Voter ID to cast your vote, and you can't vote in your own poll.</p>"

        "<h3>My Votes</h3>"
        "<p>View a history of the polls you have participated in and the options you selected. This section might also allow you to remove a previous vote if the poll is still running, this page also shows the final results of the poll if it is finished.</p>"

        "<h3>Create Poll</h3>"
        "<p>Provides tools for creating a new election or poll. You can define the poll's name, description and add the various options that users can vote for.</p>"

        "<h3>My Polls</h3>"
        "<p>View the polls you have created, edit their description, edit their options (if no one voted yet), end the poll (stop any more people from voting), delete the poll and view the people who voted.</p>"

        "<h3>Edit Profile</h3>"
        "<p>View and edit your user account information, including your real name, username, email, phone number, address and password. Keep your profile details up-to-date here.</p>"

        "<p>---------------------------------------------------------------------------------------------------------------------------------------------</p>"
        "<p>Made with love for FCIS ASU :)</p>"

        "<h4>People behind it</h4>"
        "<p>Abdo Shalaby - Mahmoud Nader - Mohamed Hany - Mohamed Hassan - Mohamed Ahmed - Mohamed Abdelaal - Abdo Hassan</p>";

	helpTextEdit->setHtml(helpContent);
	contentLayout->addWidget(helpTextEdit);

	rootLayout->addWidget(sidebar);
	rootLayout->addWidget(content);
	rootLayout->setStretch(0, 0);

}
