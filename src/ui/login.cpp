#include "login.h"
#include "db.hpp"
#include "services.hpp"
#include "utils.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>
#include <QDebug>


LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left panel: gradient background
    leftPanel = new QWidget(this);
    leftPanel->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #3366CC, stop:1 #007BFF);");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    leftLayout->setAlignment(Qt::AlignCenter);

    // Load and add logo
    const int logoSize = 200;
    QPixmap logoPixmap(":/logo.png");
    QPixmap roundedLogo = createRoundedLogo(logoPixmap, logoSize);
    if (!roundedLogo.isNull()) {
        QLabel *logoLabel = new QLabel(leftPanel);
        logoLabel->setPixmap(roundedLogo);
        logoLabel->setFixedSize(logoSize, logoSize);
        logoLabel->setStyleSheet("background-color: transparent;");
        leftLayout->addWidget(logoLabel);
    } else {
        QLabel *errorLabel = new QLabel("Logo Failed to Load", leftPanel);
        errorLabel->setAlignment(Qt::AlignCenter);
        leftLayout->addWidget(errorLabel);
    }

    // Right panel
    rightPanel = new QWidget(this);
    rightPanel->setStyleSheet("background-color: #FFFFFF;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(40, 40, 40, 40);
    rightLayout->setSpacing(20);
    rightLayout->insertStretch(0, 1);

    // Title
    QLabel *title = new QLabel("Welcome! Please Login.", rightPanel);
    title->setStyleSheet(
        "color: #003366;"
        "font-size: 24px;"
        "font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(title);

    // Username field
    userEdit = new QLineEdit(rightPanel);
    userEdit->setPlaceholderText("Username");
    userEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(userEdit);

    // Password field
    passEdit = new QLineEdit(rightPanel);
    passEdit->setPlaceholderText("Password");
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(passEdit);


    userEdit->setObjectName("userEdit");
    passEdit->setObjectName("passEdit");

    // Login button
    loginBtn = new QPushButton("Login", rightPanel);
    loginBtn->setFixedHeight(40);
    loginBtn->setStyleSheet(
        "QPushButton {"
        " background-color: #007BFF;"
        " color: #FFFFFF;"
        " font-size: 18px;"
        " border: none;"
        " border-radius: 6px;"
        " padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        " background-color: #339CFF;"
        " font-weight: bold;"
        " padding: 7px 14px;"
        "}");
    loginBtn->setCursor(Qt::PointingHandCursor);
    rightLayout->addWidget(loginBtn);

    // Register link
    registerLabel = new QLabel(rightPanel);
    registerLabel->setText(
        "<p style=\"text-align:center;\">Don't have an account? "
        "<a href=\"#register\">Register</a></p>");
    registerLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerLabel->setOpenExternalLinks(false);
    registerLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(registerLabel);
    rightLayout->addStretch(1);

    // Assemble panels
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(rightPanel, 1);

    // Connect signals
    connect(loginBtn, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(registerLabel, &QLabel::linkActivated, [this]() {
        userEdit->clear();
        passEdit->clear();
        emit registerLinkActivated();
    });

    connect(userEdit, &QLineEdit::returnPressed, [this]() {
        passEdit->setFocus();
    });


    connect(passEdit, &QLineEdit::returnPressed, this, &LoginPage::onLoginClicked);
}

void LoginPage::onLoginClicked()
{
    auto username = userEdit->text().toStdString();
    auto password = passEdit->text().toStdString();

    //std::string response =  log_in(username,password);
    std::string response = "true";
    if (response == "true")
    {
        activeUser = get_id_by_user(username);
        emit loginSuccessful();
        userEdit->clear();
        passEdit->clear();
    }
    else
    {
        QMessageBox::warning(nullptr,"Warning",QString::fromStdString(response));
    }
}
