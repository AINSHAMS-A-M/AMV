#include "register.h"
#include "utils.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent)
{
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left gradient panel
    leftPanel = new QWidget(this);
    leftPanel->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #3366CC, stop:1 #007BFF);");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    leftLayout->setAlignment(Qt::AlignCenter);

    // Logo
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

    // Right white panel
    rightPanel = new QWidget(this);
    rightPanel->setStyleSheet("background-color: #FFFFFF;");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(40, 40, 40, 40);
    rightLayout->setSpacing(20);
    rightLayout->insertStretch(0, 1);

    // Title
    QLabel *title = new QLabel("User Registration", rightPanel);
    title->setStyleSheet(
        "color: #003366;"
        "font-size: 24px;"
        "font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(title);

    // Real name field
    realNameEdit = new QLineEdit(rightPanel);
    realNameEdit->setPlaceholderText("Real name");
    realNameEdit->setStyleSheet(
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px;");
    rightLayout->addWidget(realNameEdit);

    // Username field
    usernameEdit = new QLineEdit(rightPanel);
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setStyleSheet(
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px;");
    rightLayout->addWidget(usernameEdit);

    // Password field
    passwordEdit = new QLineEdit(rightPanel);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px;");
    rightLayout->addWidget(passwordEdit);

    // Confirm password field
    confirmEdit = new QLineEdit(rightPanel);
    confirmEdit->setPlaceholderText("Confirm password");
    confirmEdit->setEchoMode(QLineEdit::Password);
    confirmEdit->setStyleSheet(
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px;");
    rightLayout->addWidget(confirmEdit);

    usernameEdit->setObjectName("usernameEdit");
    passwordEdit->setObjectName("passwordEdit");
    confirmEdit->setObjectName("confirmEdit");
    realNameEdit->setObjectName("realnameEdit");

    // Register button
    registerBtn = new QPushButton("Register", rightPanel);
    registerBtn->setFixedHeight(40);
    registerBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #007BFF;"
        "    color: #FFFFFF;"
        "    font-size: 18px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "    transition: all 0.3s ease;"
        "}"
        "QPushButton:hover {"
        "    background-color: #339CFF;"
        "    font-weight: bold;"
        "    padding: 7px 14px;"
        "}");
    registerBtn->setCursor(Qt::PointingHandCursor);
    rightLayout->addWidget(registerBtn);

    // Login link
    loginLabel = new QLabel(rightPanel);
    loginLabel->setText(
        "<p style=\"text-align:center;\">Already have an account? "
        "<a href=\"#login\">Login</a></p>");
    loginLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    loginLabel->setOpenExternalLinks(false);
    loginLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(loginLabel);
    rightLayout->addStretch(1);

    // Assemble main layout
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(rightPanel, 1);

    // Connect signals
    connect(registerBtn, &QPushButton::clicked, this, &RegisterPage::registerClicked);
    connect(loginLabel, &QLabel::linkActivated,
            this,        &RegisterPage::loginLinkActivated);
}
