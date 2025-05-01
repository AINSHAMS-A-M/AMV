#include "register.h"
#include "_structs.hpp"
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
    connect(registerBtn, &QPushButton::clicked, this, &RegisterPage::onRegisterClicked);


    connect(loginLabel, &QLabel::linkActivated, [this]() {
        emit loginLinkActivated();
        realNameEdit->clear();
        usernameEdit->clear();
        passwordEdit->clear();
        confirmEdit->clear();
    });

    connect(realNameEdit, &QLineEdit::returnPressed, [this]() {
        usernameEdit->setFocus();
    });

    connect(usernameEdit, &QLineEdit::returnPressed, [this]() {
        passwordEdit->setFocus();
    });

    connect(passwordEdit, &QLineEdit::returnPressed, [this]() {
        confirmEdit->setFocus();
    });

    connect(confirmEdit, &QLineEdit::returnPressed, this, &RegisterPage::onRegisterClicked);
}


void RegisterPage::onRegisterClicked()
{
    auto username = usernameEdit->text().toStdString();
    auto password = passwordEdit->text().toStdString();
    auto confirm = confirmEdit->text().toStdString();
    auto realname = realNameEdit->text().toStdString();

    if (username == "" || realname == "" || password == "" || confirm == "")
    {
        QMessageBox::warning(this,"Warning","Fields cannot be empty!");
    }
    else if (username.find('`') != std::string::npos|| realname.find('`') != std::string::npos)
    {
        QMessageBox::warning(this,"Warning","Invalid Character! don't type \"`\" ");
    }
    else if (password == confirm)
    {
        bool capital = 0, small = 0, number = 0, special = 0;
        for (auto ch : password)
        {
            if (ch >= 'a' && ch <= 'z') small = 1;
            else if (ch >= 'A' && ch <= 'Z') capital = 1;
            else if (ch >= '0' && ch <= '9') number = 1;
            else special = 1;
        }

        if (capital && small && number && special && password.size() >= 8)
        {
            size_t new_id = users.size();

            CreateUser newUser =
                {
                    new_id,
                    realname,
                    hash_password(password,new_id),
                    username
                };
            auto response = create_user(newUser);
            if (response == "Em")
            {
                QMessageBox::warning(this,"Warning","Fields cannot be empty!");
            }
            else if (response == "Already Exist")
            {
                QMessageBox::warning(this,"Warning","User already exists! Please login");
            }
            else if (response == "Success")
            {
                emit registrationSuccessful();
                usernameEdit->clear();
                passwordEdit->clear();
                confirmEdit->clear();
                realNameEdit->clear();
            }
        }
        else
        {
            QMessageBox::warning(this,"Warning","Invalid password, Please note that the "
                                                  "password must contain at least: a capital letter,"
                                                  "a small letter, a number, and a special character"
                                                  "(!,@,#,$,%,^,&,/,\\,,) and must at least be 8 characters!");
        }


    }
    else
    {
        QMessageBox::warning(this,"Warning","Passwords don't match!!");
    }
}
