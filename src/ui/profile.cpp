#include "profile.h"
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
#include <QLineEdit>
#include <QStackedWidget>
#include "_structs.hpp"
#include "db.hpp"
#include "mainwindow.h"
#include "QScrollArea"
#include "QScrollBar"
#include "services.hpp"
#include "qmessagebox.h"

ProfileEditPage::ProfileEditPage(QWidget *parent)
    : QWidget(parent),
    usernameField(nullptr),
    realNameField(nullptr),
    oldPasswordField(nullptr),
    newPasswordField(nullptr),
    confirmPasswordField(nullptr),
    emailField(nullptr),
    addressField(nullptr),
    phoneField(nullptr),
    sidebar(nullptr),
    content(nullptr)
{
    bgColor       = "#F5F6F8";
    cardColor     = "#FFFFFF";
    borderColor   = "#E0E0E0";
    primaryBtnColor = "#3498DB";
    primaryBtnHoverColor = "#2980B9";
    primaryBtnPressedColor = "#1F618D";
    labelColor    = "#566573";
    titleColor    = "#2C3E50";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this,"Edit Profile");

    // Main content widget
    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    // Main content layout
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(25, 25, 25, 25);
    contentLayout->setSpacing(30);

    // Page title
    QLabel *titleLabel = new QLabel("Profile", content);
    titleLabel->setStyleSheet(QString("font-size: 28px; font-weight: bold; color: %1;").arg(titleColor));
    contentLayout->addWidget(titleLabel);

    // Create stacked widget for content cards
    stackedWidget = new QStackedWidget(content);
    contentLayout->addWidget(stackedWidget);

    // Somewhere before creating your card, retrieve the active user data:
    struct UserData {
        QString username;
        QString email;
        QString address;
        QString phone;
        QString realName;
    };

    // ===== FIRST CARD: PROFILE INFORMATION WITH SCROLL AREA =====
    QScrollArea *scrollArea = new QScrollArea(stackedWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    // Style the scrollbar
    scrollArea->verticalScrollBar()->setStyleSheet(R"(
    QScrollBar:vertical {
        background: transparent;
        width: 8px;
        margin: 0px 0px 0px 0px;
    }
    QScrollBar::handle:vertical {
        background: rgba(0,0,0,0.2);
        min-height: 20px;
        border-radius: 4px;
    }
    QScrollBar::handle:vertical:hover {
        background: rgba(0,0,0,0.3);
    }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        height: 0px;
    }
)");

    // Your card
    QWidget *profileCard = new QWidget;
    profileCard->setObjectName("card");
    profileCard->setStyleSheet(
        QString("QWidget#card {"
                "    background-color: %1;"
                "    border-radius: 8px;"
                "    border: 1px solid %2;"
                "}").arg(cardColor, borderColor)
        );

    QVBoxLayout *profileCardLayout = new QVBoxLayout(profileCard);
    profileCardLayout->setContentsMargins(40, 20, 40, 50);
    profileCardLayout->setSpacing(20);

    // Title
    profileTitle = new QLabel(
        QString("View / Change Personal Information\n(current: %1)")
            .arg(activeUser.username),
        profileCard
        );
    profileTitle->setStyleSheet(
        QString("font-size: 20px; font-weight: bold; color: %1; background-color: #FFFFFF;")
            .arg(titleColor)
        );
    profileCardLayout->addWidget(profileTitle);

    // Helper lambda to create each field container
    auto makeField = [&](const QString &labelText, const QString &currentValue, QLineEdit *&outField){
        QWidget *container = new QWidget(profileCard);
        QVBoxLayout *lay = new QVBoxLayout(container);
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);


        outField = new QLineEdit(container);
        outField->setText(currentValue);
        outField->setStyleSheet(
            QString("QLineEdit {"
                    "    border: 1px solid %1;"
                    "    border-radius: 4px;"
                    "    padding: 2px;"
                    "    font-size: 14px;"
                    "    color: %2;"
                    "    background-color: %3;"
                    "}"
                    "QLineEdit:focus {"
                    "    border: 2px solid %4;"
                    "}")
                .arg(borderColor, titleColor, cardColor, primaryBtnColor)
            );
        outField->setMinimumHeight(25);


        QLabel *lbl = new QLabel(
            QString("%1").arg(labelText),
            container
            );
        lbl->setStyleSheet(
            QString("font-size: 14px; color: %1; font-weight: bold; background-color: #FFFFFF;")
                .arg(labelColor)
            );

        lay->addWidget(lbl);
        lay->addWidget(outField);
        profileCardLayout->addWidget(container);
    };

    // Create each field pre-filled
    makeField("Username",  QString::fromStdString(activeUser.username),  this->usernameField);
    makeField("Email",     QString::fromStdString(activeUser.email),     this->emailField);
    makeField("Address",   QString::fromStdString(activeUser.address),   this->addressField);
    makeField("Phone",     QString::fromStdString(activeUser.phone_number),     this->phoneField);
    makeField("Real Name", QString::fromStdString(activeUser.name),  this->realNameField);

    // Save button
    QPushButton *saveProfileBtn = new QPushButton("Save Profile", profileCard);
    saveProfileBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: %1;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 12px 25px;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
                "QPushButton:pressed {"
                "    background-color: %3;"
                "}")
            .arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );
    saveProfileBtn->setCursor(Qt::PointingHandCursor);
    saveProfileBtn->setMinimumHeight(45);
    saveProfileBtn->setMaximumWidth(150);
    QHBoxLayout *btnLay = new QHBoxLayout;
    btnLay->addWidget(saveProfileBtn);
    btnLay->addStretch();
    profileCardLayout->addLayout(btnLay);

    // Put the card into the scroll area and then into the stacked widget
    scrollArea->setWidget(profileCard);
    stackedWidget->addWidget(scrollArea);


    // ===== SECOND CARD: PASSWORD CHANGE =====
    QWidget *passwordCard = new QWidget(stackedWidget);
    passwordCard->setObjectName("card");
    passwordCard->setStyleSheet(
        QString("QWidget#card {"
                "    background-color: %1;"
                "    border-radius: 8px;"
                "    border: 1px solid %2;"
                "}").arg(cardColor, borderColor)
        );

    QVBoxLayout *passwordCardLayout = new QVBoxLayout(passwordCard);
    passwordCardLayout->setContentsMargins(30, 30, 30, 30);
    passwordCardLayout->setSpacing(20);

    // Card title - without grey background
    QLabel *passwordTitle = new QLabel("Change Password", passwordCard);
    passwordTitle->setStyleSheet(QString("font-size: 20px; font-weight: bold; color: %1; background-color: #FFFFFF;").arg(titleColor));
    passwordCardLayout->addWidget(passwordTitle);

    // Old Password field
    QWidget *oldPasswordContainer = new QWidget(passwordCard);
    QVBoxLayout *oldPasswordLayout = new QVBoxLayout(oldPasswordContainer);
    oldPasswordLayout->setContentsMargins(0, 0, 0, 0);
    oldPasswordLayout->setSpacing(0);

    QLabel *oldPasswordLabel = new QLabel("Current Password", oldPasswordContainer);
    oldPasswordLabel->setStyleSheet(QString("font-size: 14px; color: %1; font-weight: bold; background-color: #FFFFFF;").arg(labelColor));
    oldPasswordField = new QLineEdit(oldPasswordContainer);
    oldPasswordField->setEchoMode(QLineEdit::Password);
    oldPasswordField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 2px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    oldPasswordField->setMinimumHeight(40);
    oldPasswordLayout->addWidget(oldPasswordLabel);
    oldPasswordLayout->addWidget(oldPasswordField);
    passwordCardLayout->addWidget(oldPasswordContainer);

    // New Password field
    QWidget *newPasswordContainer = new QWidget(passwordCard);
    QVBoxLayout *newPasswordLayout = new QVBoxLayout(newPasswordContainer);
    newPasswordLayout->setContentsMargins(0, 0, 0, 0);
    newPasswordLayout->setSpacing(0);

    QLabel *newPasswordLabel = new QLabel("New Password", newPasswordContainer);
    newPasswordLabel->setStyleSheet(QString("font-size: 14px; color: %1; font-weight: bold;  background-color: #FFFFFF;").arg(labelColor));
    newPasswordField = new QLineEdit(newPasswordContainer);
    newPasswordField->setEchoMode(QLineEdit::Password);
    newPasswordField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 2px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    newPasswordField->setMinimumHeight(40);
    newPasswordLayout->addWidget(newPasswordLabel);
    newPasswordLayout->addWidget(newPasswordField);
    passwordCardLayout->addWidget(newPasswordContainer);

    // Confirm Password field
    QWidget *confirmPasswordContainer = new QWidget(passwordCard);
    QVBoxLayout *confirmPasswordLayout = new QVBoxLayout(confirmPasswordContainer);
    confirmPasswordLayout->setContentsMargins(0, 0, 0, 0);
    confirmPasswordLayout->setSpacing(0);

    QLabel *confirmPasswordLabel = new QLabel("Confirm New Password", confirmPasswordContainer);
    confirmPasswordLabel->setStyleSheet(QString("font-size: 14px; color: %1; font-weight: bold; background-color: #FFFFFF;").arg(labelColor));
    confirmPasswordField = new QLineEdit(confirmPasswordContainer);
    confirmPasswordField->setEchoMode(QLineEdit::Password);
    confirmPasswordField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 2px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    confirmPasswordField->setMinimumHeight(40);
    confirmPasswordLayout->addWidget(confirmPasswordLabel);
    confirmPasswordLayout->addWidget(confirmPasswordField);
    passwordCardLayout->addWidget(confirmPasswordContainer);

    // Change Password button
    QPushButton *changePasswordBtn = new QPushButton("Change Password", passwordCard);
    changePasswordBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: %1;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 12px 25px;"
                "    font-size: 14px;"
                "    font-weight: bold;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
                "QPushButton:pressed {"
                "    background-color: %3;"
                "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );
    changePasswordBtn->setCursor(Qt::PointingHandCursor);
    changePasswordBtn->setMinimumHeight(45);
    changePasswordBtn->setMaximumWidth(200);

    // Add button to layout with alignment
    QHBoxLayout *passwordBtnLayout = new QHBoxLayout();
    passwordBtnLayout->addWidget(changePasswordBtn);
    passwordBtnLayout->addStretch();
    passwordCardLayout->addLayout(passwordBtnLayout);

    // Add the card to the stacked widget
    stackedWidget->addWidget(passwordCard);

    // Create navigation buttons for the stacked widget
    QWidget *navContainer = new QWidget(content);
    QHBoxLayout *navLayout = new QHBoxLayout(navContainer);
    navLayout->setContentsMargins(0, 20, 0, 0);
    navLayout->setSpacing(10);

    profileInfoBtn = new QPushButton("Profile Info", navContainer);
    profileInfoBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: %1;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
                "QPushButton:pressed {"
                "    background-color: %3;"
                "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );
    profileInfoBtn->setCursor(Qt::PointingHandCursor);

    changePasswordTabBtn = new QPushButton("Change Password", navContainer);

    changePasswordTabBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: transparent;"
                "    color: %1;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    color: %2;"
                "}"
                "QPushButton:pressed {"
                "    color: %3;"
                "}").arg(labelColor, titleColor, primaryBtnColor)
        );
    changePasswordTabBtn->setCursor(Qt::PointingHandCursor);

    navLayout->addWidget(profileInfoBtn);
    navLayout->addWidget(changePasswordTabBtn);
    navLayout->addStretch();

    contentLayout->insertWidget(1, navContainer);

    // Connect buttons to switch between stacked widget pages
    connect(profileInfoBtn, &QPushButton::clicked, this, &ProfileEditPage::onEditProfileClicked);

    connect(changePasswordTabBtn, &QPushButton::clicked, [=]() {

        usernameField->setText(QString::fromStdString(activeUser.username));
        emailField->setText(QString::fromStdString(activeUser.email));
        addressField->setText(QString::fromStdString(activeUser.address));
        phoneField->setText(QString::fromStdString(activeUser.phone_number));
        realNameField->setText(QString::fromStdString(activeUser.name));
        oldPasswordField->clear();
        newPasswordField->clear();
        confirmPasswordField->clear();
        oldPasswordField->setFocus();
        stackedWidget->setCurrentIndex(1);

        // Style for the active tab
        changePasswordTabBtn->setStyleSheet(
            QString("QPushButton {"
                    "    background-color: %1;"
                    "    color: white;"
                    "    border: none;"
                    "    border-radius: 8px;;"
                    "    padding: 8px 15px;"
                    "    font-size: 14px;"
                    "}"
                    "QPushButton:hover {"
                    "    background-color: %2;"
                    "}"
                    "QPushButton:pressed {"
                    "    background-color: %3;"
                    "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
            );

        // Style for the inactive tab (profile tab)
        profileInfoBtn->setStyleSheet(
            QString("QPushButton {"
                    "    background-color: transparent;"
                    "    color: %1;"
                    "    border: none;"
                    "    border-radius: 8px;"
                    "    padding: 8px 15px;"
                    "    font-size: 14px;"
                    "}"
                    "QPushButton:hover {"
                    "    color: %2;"
                    "}"
                    "QPushButton:pressed {"
                    "    color: %3;"
                    "}").arg(labelColor, titleColor, primaryBtnColor)
            );
    });

    // Set initial page and corresponding button style
    stackedWidget->setCurrentIndex(0);
    profileInfoBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: %1;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
                "QPushButton:pressed {"
                "    background-color: %3;"
                "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );
    changePasswordTabBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: transparent;"
                "    color: %1;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    color: %2;"
                "}"
                "QPushButton:pressed {"
                "    color: %3;"
                "}").arg(labelColor, titleColor, primaryBtnColor)
        );

    connect(saveProfileBtn, &QPushButton::clicked, this, &ProfileEditPage::onSaveProfileClicked);
    connect(changePasswordBtn, &QPushButton::clicked, this, &ProfileEditPage::onUpdatePasswordClicked);
    connect(oldPasswordField, &QLineEdit::returnPressed, [this]() {
        newPasswordField->setFocus();
    });

    connect(newPasswordField, &QLineEdit::returnPressed, [this]() {
        confirmPasswordField->setFocus();
    });
    connect(confirmPasswordField, &QLineEdit::returnPressed, this, &ProfileEditPage::onUpdatePasswordClicked);

    contentLayout->addStretch();

    // Add the sidebar and content to the main layout
    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    rootLayout->setStretch(0, 0);
}

void ProfileEditPage::onSaveProfileClicked()
{
    EditUser edited = {
        activeUser.id,
        activeUser.username,
        activeUser.name,
        activeUser.email,
        activeUser.phone_number,
        activeUser.address
    };

    std::string new_username = this->usernameField->text().toStdString();
    std::string new_realname = this->realNameField->text().toStdString();
    std::string address = this->addressField->text().toStdString();
    std::string email = this->emailField->text().toStdString();
    std::string phone = this->phoneField->text().toStdString();

    if ((new_username.empty() || new_username == activeUser.username) &&
        (new_realname.empty() || new_realname == activeUser.name) &&
        (address.empty() || address == activeUser.address) &&
        (email.empty() || email == activeUser.email) &&
        (phone.empty() || phone == activeUser.phone_number))
    {
        QMessageBox::warning(nullptr,"Warning","Nothing Changed!");
        return;
    }

    if (!new_username.empty())
    {
        for (auto ch : new_username)
        {
            if (ch == ',')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected \",\"");
                return;
            }
        }
        edited.new_username = new_username;
    }
    if (!new_realname.empty())
    {
        for (auto ch : new_realname)
        {
            if (ch == '`')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected \",\"");
                return;
            }
        }
        edited.new_real_name = new_realname;
    }

    if (!address.empty())
    {
        for (auto ch : address)
        {
            if (ch == '`')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected \",\"");
                return;
            }
        }
        edited.address = address;
    }

    if (!phone.empty())
    {
        for (auto ch : phone)
        {
            if (ch == '`')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected in Phone \",\"");
                return;
            }
        }
        edited.phone_number = phone;
    }

    if (!email.empty())
    {
        for (auto ch : email)
        {
            if (ch == '`')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected in Email \",\"");
                return;
            }
        }
        edited.email = email;
    }


    auto response = edit_user(edited);
    if (response == "done")
    {
        QMessageBox::information(nullptr,"Success","Information Updated Successfully!");
        emit changeWelcomeLabel();
    }

    else QMessageBox::warning(nullptr,"Warning", QString::fromStdString(response));

    usernameField->setText(QString::fromStdString(activeUser.username));
    emailField->setText(QString::fromStdString(activeUser.email));
    addressField->setText(QString::fromStdString(activeUser.address));
    phoneField->setText(QString::fromStdString(activeUser.phone_number));
    realNameField->setText(QString::fromStdString(activeUser.name));
    profileTitle->setText(QString("View / Change Personal Information\n(current: %1)")
                              .arg(activeUser.username));

    oldPasswordField->clear();
    newPasswordField->clear();
    confirmPasswordField->clear();
    usernameField->setFocus();
}

void ProfileEditPage::onUpdatePasswordClicked()
{

    std::string old_password = this->oldPasswordField->text().toStdString();
    std::string new_password = this->newPasswordField->text().toStdString();
    std::string confirm = this->confirmPasswordField->text().toStdString();

    usernameField->setText(QString::fromStdString(activeUser.username));
    emailField->setText(QString::fromStdString(activeUser.email));
    addressField->setText(QString::fromStdString(activeUser.address));
    phoneField->setText(QString::fromStdString(activeUser.phone_number));
    realNameField->setText(QString::fromStdString(activeUser.name));

    oldPasswordField->clear();
    newPasswordField->clear();
    confirmPasswordField->clear();
    oldPasswordField->setFocus();

    if (old_password.empty() || new_password.empty() || confirm.empty())
    {
        QMessageBox::warning(nullptr,"Warning","An Empty Input field is detected!");
        return;
    }
    else if (new_password != confirm)
    {
        QMessageBox::warning(nullptr,"Warning","Passwords don't match!");
        return;
    }
    else if (hash_password(new_password,activeUser.id) == activeUser.hashed_password)
    {
        QMessageBox::warning(nullptr,"Warning","New password can't be the same as old one!");
        return;
    }

    bool capital = 0, small = 0, number = 0, special = 0;
    for (auto ch : new_password)
    {
        if (ch >= 'a' && ch <= 'z') small = 1;
        else if (ch >= 'A' && ch <= 'Z') capital = 1;
        else if (ch >= '0' && ch <= '9') number = 1;
        else special = 1;
    }
    if (!capital || !small || !number || !special || new_password.size() < 8)
    {
        QMessageBox::warning(nullptr,"Warning","Invalid password, Please note that the "
                                                 "password must contain at least: a capital letter,"
                                                 "a small letter, a number, and a special character"
                                                 "(!,@,#,$,%,^,&,/,\\,,) and must at least be 8 characters!");
        return;
    }

    EditUserPassword edited = {
        activeUser.id,
        hash_password(old_password,activeUser.id),
        new_password
    };

    std::string response = edit_user_password(edited);
    if (response == "done")
    {
        QMessageBox::information(nullptr,"Success","Password Changed Successfully!");
    }
    else
    {
        QMessageBox::warning(nullptr,"Warning",QString::fromStdString(response));
    }

}

void ProfileEditPage::onEditProfileClicked()
{
    oldPasswordField->clear();
    newPasswordField->clear();
    confirmPasswordField->clear();
    usernameField->setFocus();
    stackedWidget->setCurrentIndex(0);

    // Style for the active tab
    profileInfoBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: %1;"
                "    color: white;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    background-color: %2;"
                "}"
                "QPushButton:pressed {"
                "    background-color: %3;"
                "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );

    // Style for the inactive tab (password tab)
    changePasswordTabBtn->setStyleSheet(
        QString("QPushButton {"
                "    background-color: transparent;"
                "    color: %1;"
                "    border: none;"
                "    border-radius: 8px;"
                "    padding: 8px 15px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    color: %2;"
                "}"
                "QPushButton:pressed {"
                "    color: %3;"
                "}").arg(labelColor, titleColor, primaryBtnColor)
        );
}
