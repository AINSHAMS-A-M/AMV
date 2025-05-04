#include "sidebar.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QHBoxLayout>
#include "QApplication"
#include "QMainWindow"
#include "QMessageBox"
#include "utils.h"
#include "nav.h"
#include "db.hpp"


SidebarWidget::SidebarWidget(QWidget *parent, std::string page)
    : QWidget(parent)
{
    const QString sidebarColor = "#2C3E50";
    this->setFixedWidth(200);
    this->setStyleSheet(QString("SidebarWidget { background-color: %1; }").arg(sidebarColor));
    this->setAttribute(Qt::WA_StyledBackground);

    sbLayout = new QVBoxLayout(this);
    sbLayout->setSpacing(15);
    sbLayout->setContentsMargins(20, 20, 20, 20);

    // Logo
    const int logoSize = 120;
    QPixmap logoPixmap(":/logo.png");
    QPixmap roundedLogo = createRoundedLogo(logoPixmap, logoSize);
    if (!roundedLogo.isNull())
    {
        QLabel *logoLabel = new QLabel(this);
        logoLabel->setPixmap(roundedLogo);
        logoLabel->setFixedSize(logoSize, logoSize);
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setStyleSheet("background-color: transparent;");
        sbLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);
    }

    setupMenuItems(page);
    setupWelcomeLabel();
    setupLogoutButton();

    sbLayout->addStretch();
}

void SidebarWidget::setupMenuItems(std::string page)
{
    QStringList menuItems = {"Help", "Vote", "My Votes", "Create Poll", "My Polls", "Profile"};
    for (const QString &item : menuItems) {
        QPushButton *btn = new QPushButton(item, this);
        btn->setStyleSheet(
            "QPushButton { color: white; background: none; border: none; text-align: left; font-size: 18px; padding: 10px; border-radius: 4px; }"
            "QPushButton:hover { background-color: rgba(255,255,255,0.1); }"
            );
        btn->setCursor(Qt::PointingHandCursor);

        if (item == page) {
            btn->setStyleSheet(
                "QPushButton { color: #333333; background-color: rgba(255,255,255,0.2); border: none; text-align: left; font-size: 18px; padding: 10px; font-weight: bold; }"
                "QPushButton:hover { background-color: rgba(255,255,255,0.3); }"
                );
        }

        connect(btn, &QPushButton::clicked, [item]() {
            if (item == "Help") NavigationManager::instance().navigate(NavigationManager::Help);
            else if (item == "Vote") NavigationManager::instance().navigate(NavigationManager::Vote);
            else if (item == "My Votes") NavigationManager::instance().navigate(NavigationManager::MyVotes);
            else if (item == "Create Poll") NavigationManager::instance().navigate(NavigationManager::CreatePoll);
            else if (item == "My Polls") NavigationManager::instance().navigate(NavigationManager::MyPolls);
            else if (item == "Profile") NavigationManager::instance().navigate(NavigationManager::Profile);
        });

        sbLayout->addWidget(btn);
    }
}

void SidebarWidget::setupWelcomeLabel()
{
    welcomeLabel = new QLabel("Welcome\n" + QString::fromStdString(activeUser.name) + "!", this);
    welcomeLabel->setStyleSheet("color: #EBECF0; font-size: 15px; padding: 5px 10px; font-style: italic; font-weight: bold;");
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setAlignment(Qt::AlignLeft);
    layout()->addWidget(welcomeLabel);
}


void SidebarWidget::setupLogoutButton()
{
    QPushButton *logoutBtn = new QPushButton("Logout", this);
    logoutBtn->setStyleSheet(
        "QPushButton { color: white; background-color: #E74C3C; border: none; "
        "text-align: center; font-size: 16px; padding: 8px 12px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #C0392B; }"
        );
    logoutBtn->setCursor(Qt::PointingHandCursor);

    // Connect logout button to a slot that will handle the window reset
    connect(logoutBtn, &QPushButton::clicked, this, [this]() {
        auto reply = QMessageBox::question(
            this,
            "Log out",
            QString("Are you sure you want to log out?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );
        if (reply == QMessageBox::Yes)
        {
            // Clear the active user
            activeUser = {};
            // Emit signal for logout
            NavigationManager::instance().navigate(NavigationManager::Logout);
        }

    });

    // Add to the layout - note we add it after the welcome label but before the stretch
    sbLayout->addWidget(logoutBtn);
}
