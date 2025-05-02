#include "profile.h"
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
#include "services.hpp"
#include "qmessagebox.h"

ProfileEditPage::ProfileEditPage(QWidget *parent)
    : QWidget(parent),
    sidebar(nullptr),
    content(nullptr),
    usernameField(nullptr),
    realNameField(nullptr),
    oldPasswordField(nullptr),
    newPasswordField(nullptr),
    confirmPasswordField(nullptr)
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
    contentLayout->setContentsMargins(40, 40, 40, 40);
    contentLayout->setSpacing(30);

    // Page title
    QLabel *titleLabel = new QLabel("Edit Profile", content);
    titleLabel->setStyleSheet(QString("font-size: 28px; font-weight: bold; color: %1;").arg(titleColor));
    contentLayout->addWidget(titleLabel);

    // Create stacked widget for content cards
    stackedWidget = new QStackedWidget(content);
    contentLayout->addWidget(stackedWidget);

    // ===== FIRST CARD: PROFILE INFORMATION =====
    QWidget *profileCard = new QWidget(stackedWidget);
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

    QLabel *profileTitle = new QLabel("Change Personal Information", profileCard);
    profileTitle->setStyleSheet(QString("font-size: 20px; font-weight: bold; color: %1; background-color: #FFFFFF;").arg(titleColor));
    profileCardLayout->addWidget(profileTitle);

    // Username field
    QWidget *usernameContainer = new QWidget(profileCard);
    QVBoxLayout *usernameLayout = new QVBoxLayout(usernameContainer);
    usernameLayout->setContentsMargins(0, 0, 0, 0);
    usernameLayout->setSpacing(0);

    QLabel *usernameLabel = new QLabel("Username", usernameContainer);
    usernameLabel->setStyleSheet(QString("font-size: 14px; color: %1; font-weight: bold; background-color: #FFFFFF;").arg(labelColor));
    this->usernameField = new QLineEdit(usernameContainer);
    this->usernameField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 10px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    this->usernameField->setMinimumHeight(40);
    this->usernameField->setObjectName("userEdit");
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(this->usernameField);
    profileCardLayout->addWidget(usernameContainer);

    // Real name field
    QWidget *realNameContainer = new QWidget(profileCard);
    QVBoxLayout *realNameLayout = new QVBoxLayout(realNameContainer);
    realNameLayout->setContentsMargins(0, 0, 0, 0);
    realNameLayout->setSpacing(0);

    QLabel *realNameLabel = new QLabel("Real Name", realNameContainer);
    realNameLabel->setStyleSheet(QString("font-size: 14px; color: %1; font-weight: bold; background-color: #FFFFFF;").arg(labelColor));
    this->realNameField = new QLineEdit(realNameContainer);
    this->realNameField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 10px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    this->realNameField->setMinimumHeight(40);
    this->realNameField->setObjectName("realEdit");
    realNameLayout->addWidget(realNameLabel);
    realNameLayout->addWidget(this->realNameField);
    profileCardLayout->addWidget(realNameContainer);

    // Save profile button
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
                "}").arg(primaryBtnColor, primaryBtnHoverColor, primaryBtnPressedColor)
        );
    saveProfileBtn->setCursor(Qt::PointingHandCursor);
    saveProfileBtn->setMinimumHeight(45);
    saveProfileBtn->setMaximumWidth(150);

    // Add button to layout with alignment
    QHBoxLayout *profileBtnLayout = new QHBoxLayout();
    profileBtnLayout->addWidget(saveProfileBtn);
    profileBtnLayout->addStretch();
    profileCardLayout->addLayout(profileBtnLayout);

    // Add the card to the stacked widget
    stackedWidget->addWidget(profileCard);

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
    this->oldPasswordField->setObjectName("oldPassEdit");
    oldPasswordField->setStyleSheet(
        QString("QLineEdit {"
                "    border: 1px solid %1;"
                "    border-radius: 4px;"
                "    padding: 10px;"
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
                "    padding: 10px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    newPasswordField->setMinimumHeight(40);
    this->newPasswordField->setObjectName("newPassEdit");
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
                "    padding: 10px;"
                "    font-size: 14px;"
                "    color: %2;"
                "    background-color: %3;"
                "}"
                "QLineEdit:focus {"
                "    border: 2px solid %4;"
                "}").arg(borderColor, titleColor, cardColor, primaryBtnColor)
        );
    confirmPasswordField->setMinimumHeight(40);
    this->confirmPasswordField->setObjectName("confirmPassEdit");
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
        usernameField->clear();
        realNameField->clear();
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
        activeUser.name
    };

    std::string new_username = this->usernameField->text().toStdString();
    std::string new_realname = this->realNameField->text().toStdString();
    usernameField->clear();
    realNameField->clear();
    oldPasswordField->clear();
    newPasswordField->clear();
    confirmPasswordField->clear();
    usernameField->setFocus();
    if (new_username.empty() && new_realname.empty())
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
            if (ch == ',')
            {
                QMessageBox::warning(nullptr,"Warning","Invalid Character detected \",\"");
                return;
            }
        }
        edited.new_real_name = new_realname;
    }
    auto response = edit_user(edited);
    if (response == "done")
        QMessageBox::information(nullptr,"Success","Information Updated Successfully!");
    else QMessageBox::warning(nullptr,"Warning", QString::fromStdString(response));
}

void ProfileEditPage::onUpdatePasswordClicked()
{

    std::string old_password = this->oldPasswordField->text().toStdString();
    std::string new_password = this->newPasswordField->text().toStdString();
    std::string confirm = this->confirmPasswordField->text().toStdString();
    usernameField->clear();
    realNameField->clear();
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
    usernameField->clear();
    realNameField->clear();
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
                "    background-color: transparent;" // Removed grey background
                "    color: %1;" // Changed text color to labelColor
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
