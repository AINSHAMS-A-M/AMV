#include "register.h"
#include "_structs.hpp"
#include "db.hpp"
#include "services.hpp"
#include "utils.h"
#include <regex>
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
    if (!roundedLogo.isNull())
    {
        QLabel *logoLabel = new QLabel(leftPanel);
        logoLabel->setPixmap(roundedLogo);
        logoLabel->setFixedSize(logoSize, logoSize);
        logoLabel->setStyleSheet("background-color: transparent;");
        leftLayout->addWidget(logoLabel);
    }
    else
    {
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
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(realNameEdit);

    // Real name field
    phoneNumberEdit = new QLineEdit(rightPanel);
    phoneNumberEdit->setPlaceholderText("Phone Number");
    phoneNumberEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(phoneNumberEdit);

    // Email field
    emailEdit = new QLineEdit(rightPanel);
    emailEdit->setPlaceholderText("Email");
    emailEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(emailEdit);

    // Address field
    addressEdit = new QLineEdit(rightPanel);
    addressEdit->setPlaceholderText("Address");
    addressEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(addressEdit);

    // Username field
    usernameEdit = new QLineEdit(rightPanel);
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(usernameEdit);

    // Password field
    passwordEdit = new QLineEdit(rightPanel);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(passwordEdit);

    // Confirm password field
    confirmEdit = new QLineEdit(rightPanel);
    confirmEdit->setPlaceholderText("Confirm password");
    confirmEdit->setEchoMode(QLineEdit::Password);
    confirmEdit->setStyleSheet(
        "QLineEdit {"
        "padding: 10px;"
        "font-size: 16px;"
        "border: 1px solid #DADCE0;"
        "border-radius: 6px; }"
        "QLineEdit:focus {"
        "    border: 2px solid #3498DB;"
        "}");
    rightLayout->addWidget(confirmEdit);

    // Register button
    registerBtn = new QPushButton("Register", rightPanel);
    registerBtn->setFixedHeight(40);
    registerBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498DB;"
        "    color: #FFFFFF;"
        "    font-size: 18px;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980B9;"
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

    connect(loginLabel, &QLabel::linkActivated, [this]()
            {
        emit loginLinkActivated();
        realNameEdit->clear();
        usernameEdit->clear();
        passwordEdit->clear();
        emailEdit->clear();
        addressEdit->clear();
        phoneNumberEdit->clear();
        confirmEdit->clear(); });

    connect(realNameEdit, &QLineEdit::returnPressed, [this]()
            { phoneNumberEdit->setFocus(); });

    connect(phoneNumberEdit, &QLineEdit::returnPressed, [this]()
            { emailEdit->setFocus(); });

    connect(emailEdit, &QLineEdit::returnPressed, [this]()
            { addressEdit->setFocus(); });

    connect(addressEdit, &QLineEdit::returnPressed, [this]()
            { usernameEdit->setFocus(); });

    connect(usernameEdit, &QLineEdit::returnPressed, [this]()
            { passwordEdit->setFocus(); });

    connect(passwordEdit, &QLineEdit::returnPressed, [this]()
            { confirmEdit->setFocus(); });

    connect(confirmEdit, &QLineEdit::returnPressed, this, &RegisterPage::onRegisterClicked);
}

void RegisterPage::onRegisterClicked()
{
    auto username = usernameEdit->text().toStdString();
    auto password = passwordEdit->text().toStdString();
    auto confirm = confirmEdit->text().toStdString();
    auto realname = realNameEdit->text().toStdString();
    auto email = emailEdit->text().toStdString();
    auto phone = phoneNumberEdit->text().toStdString();
    auto address = addressEdit->text().toStdString();

    if (username == "" || realname == "" || password == "" || confirm == "" || phone == "" || address == "" || email == "")
    {
        QMessageBox::warning(this, "Warning", "Fields cannot be empty!");
    }
    else if (
        username.find('`') != std::string::npos ||
        realname.find('`') != std::string::npos ||
        phone.find('`') != std::string::npos ||
        address.find('`') != std::string::npos ||
        email.find('`') != std::string::npos)
    {
        QMessageBox::warning(this, "Warning", "Invalid Character! don't type \"`\" ");
    }
    else if (password == confirm)
    {
        std::regex email_pattern(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
        std::regex phone_pattern(R"(^0(10|11|12|15)\d{8}$)");
        std::regex password_pattern(R"(^(?=.*[!@#$%^&*()\-_=+{}[\]|;:'",.<>/?~`])(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z0-9!@#$%^&*()\-_=+{}[\]|;:'",.<>/?~`]{8,}$)");
        // old password validation logic
        // bool capital = 0,small = 0, number = 0, special = 0;
        // for (auto ch : password)
        // {
        //     if (ch >= 'a' && ch <= 'z')
        //         small = 1;
        //     else if (ch >= 'A' && ch <= 'Z')
        //         capital = 1;
        //     else if (ch >= '0' && ch <= '9')
        //         number = 1;
        //     else
        //         special = 1;
        // }capital && small && number && special && password.size() >= 8

        if (std::regex_match(password, password_pattern) && std::regex_match(email, email_pattern) && std::regex_match(phone, phone_pattern))
        {
            size_t new_id = users.size();

            CreateUser newUser = {
                new_id,
                realname,
                hash_password(password, new_id),
                username,
                email,
                phone,
                address,
            };
            auto response = create_user(newUser);
            if (response == "Em")
            {
                QMessageBox::warning(this, "Warning", "Fields cannot be empty!");
            }
            else if (response == "Already Exist")
            {
                QMessageBox::warning(this, "Warning", "User already exists! Please login");
            }
            else if (response == "Success")
            {
                emit registrationSuccessful();
                usernameEdit->clear();
                passwordEdit->clear();
                confirmEdit->clear();
                realNameEdit->clear();
                addressEdit->clear();
                emailEdit->clear();
                phoneNumberEdit->clear();
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Invalid password or email or phone number , Please note that the "
                                                  "password must contain at least: a capital letter,"
                                                  "a small letter, a number, and a special character"
                                                  "(!,@,#,$,%,^,&,/,\\,,) and must at least be 8 characters!");
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Passwords don't match!!");
    }
}
