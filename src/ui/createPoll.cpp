#include "createPoll.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QScrollArea>
#include "mainwindow.h"
#include "qmessagebox"
#include "services.hpp"

CreatePollPage::CreatePollPage(QWidget *parent)
    : QWidget(parent)
    , sidebar(nullptr)
    , content(nullptr)
    , optionsScrollArea(nullptr)
    , pollNameEdit(nullptr)
    , pollDescEdit(nullptr)
    , voterIdEdit(nullptr)
    , optionsContainer(nullptr)
    , optionsLayout(nullptr)
{
    const QString bgColor = "#F5F6F8";

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    sidebar = new SidebarWidget(this,"Create Poll");


    // ================= Content Area =================
    content = new QWidget(this);
    content->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel("Create New Poll");
    titleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 26px;"
        "  font-weight: 600;"
        "  color: #2D3748;"
        "  margin-bottom: 25px;"
        "}"
        );
    contentLayout->addWidget(titleLabel);

    // Form Fields
    QWidget *formWidget = new QWidget();
    QFormLayout *formLayout = new QFormLayout(formWidget);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setSpacing(20);

    const QString inputStyle =
        "QLineEdit, QTextEdit {"
        "  background: white;"
        "  border: 2px solid #E2E8F0;"
        "  border-radius: 8px;"
        "  padding: 14px;"
        "  font-size: 14px;"
        "  color: #4A5568;"
        "}"
        "QLineEdit:focus, QTextEdit:focus {"
        "  border-color: #667EEA;"
        "  outline: none;"
        "}"
        "QLineEdit::placeholder, QTextEdit::placeholder {"
        "  color: #A0AEC0;"
        "  font-style: italic;"
        "}";

    pollNameEdit = new QLineEdit();
    pollNameEdit->setPlaceholderText("Enter poll title...");
    pollNameEdit->setStyleSheet(inputStyle);
    formLayout->addRow("Poll Name:", pollNameEdit);

    pollDescEdit = new QTextEdit();
    pollDescEdit->setPlaceholderText("Enter poll description...");
    pollDescEdit->setStyleSheet(inputStyle);
    pollDescEdit->setFixedHeight(100);
    formLayout->addRow("Description:", pollDescEdit);

    voterIdEdit = new QLineEdit();
    voterIdEdit->setPlaceholderText("Enter voter ID...");
    voterIdEdit->setStyleSheet(inputStyle);
    formLayout->addRow("Voter ID:", voterIdEdit);

    contentLayout->addWidget(formWidget);

    // Scrollable Options Area
    optionsScrollArea = new QScrollArea(content);
    optionsScrollArea->setWidgetResizable(true);
    optionsScrollArea->setStyleSheet(
        "QScrollArea {"
        "  border: none;"
        "  background: transparent;"
        "}"
        "QScrollBar:vertical {"
        "  width: 10px;"
        "  background: #F8FAFC;"
        "  border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #CBD5E0;"
        "  border-radius: 4px;"
        "  min-height: 40px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0px;"
        "}"
        );

    optionsContainer = new QWidget();
    optionsLayout = new QVBoxLayout(optionsContainer);
    optionsLayout->setContentsMargins(2, 2, 12, 2);
    optionsLayout->setSpacing(18);

    optionsScrollArea->setWidget(optionsContainer);
    contentLayout->addWidget(optionsScrollArea, 1);

    // Button Container
    QWidget *buttonContainer = new QWidget();
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 20, 0, 0);
    buttonLayout->setSpacing(15);

    // Add Option Button
    QPushButton *addOptionBtn = new QPushButton("Add Option");
    addOptionBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #667EEA;"
        "  color: white;"
        "  padding: 14px 28px;"
        "  border-radius: 8px;"
        "  font-size: 15px;"
        "  font-weight: 500;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #5A67D8;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #4C51BF;"
        "}"
        );
    connect(addOptionBtn, &QPushButton::clicked, this, &CreatePollPage::onAddOptionClicked);
    addOptionBtn->setCursor(Qt::PointingHandCursor);

    // Create Poll Button
    QPushButton *createBtn = new QPushButton("Create Poll");
    createBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #48BB78;"
        "  color: white;"
        "  padding: 16px 32px;"
        "  border-radius: 8px;"
        "  font-size: 16px;"
        "  font-weight: 600;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #38A169;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2F855A;"
        "}"
        );
    connect(createBtn, &QPushButton::clicked, this, &CreatePollPage::onCreatePollBtnClicked);
    createBtn->setDefault(true);
    createBtn->setCursor(Qt::PointingHandCursor);

    buttonLayout->addWidget(addOptionBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(createBtn);
    contentLayout->addWidget(buttonContainer);

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(content);
    connect(pollNameEdit, &QLineEdit::returnPressed, [this]()
            {
                voterIdEdit->setFocus();
            });
}

void CreatePollPage::addOptionWidget()
{
    QWidget *optWidget = new QWidget(optionsContainer);
    optWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *optLayout = new QHBoxLayout(optWidget);
    optLayout->setContentsMargins(0, 0, 0, 0);
    optLayout->setSpacing(15);

    QLineEdit *nameEdit = new QLineEdit(optWidget);
    nameEdit->setPlaceholderText("Option name");
    nameEdit->setStyleSheet(
        "QLineEdit {"
        "  background: white;"
        "  border: 2px solid #E2E8F0;"
        "  border-radius: 6px;"
        "  padding: 12px;"
        "  font-size: 15px;"
        "  color: #4A5568;"
        "  min-width: 220px;"
        "}"
        "QLineEdit:focus {"
        "  border-color: #667EEA;"
        "}"
        );

    QPushButton *removeBtn = new QPushButton("Remove");
    removeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FC8181;"
        "  color: #63171B;"
        "  padding: 10px 18px;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: 500;"
        "  border: none;"
        "}"
        "QPushButton:hover {"
        "  background-color: #F56565;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #E53E3E;"
        "}"
        );

    optLayout->addWidget(nameEdit, 1);
    optLayout->addWidget(removeBtn);

    optionsLayout->addWidget(optWidget);
    optionWidgets.append(optWidget);

    connect(removeBtn, &QPushButton::clicked, this, &CreatePollPage::onRemoveOptionClicked);
    removeBtn->setCursor(Qt::PointingHandCursor);
}

void CreatePollPage::onRemoveOptionClicked()
{
    if (QPushButton *btn = qobject_cast<QPushButton*>(sender()))
    {
        if (QWidget *optWidget = btn->parentWidget())
        {
            optionsLayout->removeWidget(optWidget);
            optionWidgets.removeOne(optWidget);
            optWidget->deleteLater();
        }
    }
}

void CreatePollPage::onAddOptionClicked()
{
    addOptionWidget();
}

void CreatePollPage::onCreatePollBtnClicked()
{
    auto pollName = pollNameEdit->text().toStdString();
    auto pollDesc = pollDescEdit->toPlainText().toStdString();
    auto voterId  = voterIdEdit->text().toStdString();
    MeshVector<std::string> options;

    if (pollName.find('`') != std::string::npos || pollDesc.find('`') != std::string::npos || voterId.find('`') != std::string::npos || pollDesc.find('\n') != std::string::npos)
    {
        QMessageBox::warning(nullptr, "Warning", "Invalid character detected!");
        return;
    }

    for (QWidget *optWidget : optionWidgets)
    {
        auto nameEdit = optWidget->findChild<QLineEdit*>()->text().toStdString();
        if (nameEdit.find('`') != std::string::npos)
        {
            QMessageBox::warning(nullptr, "Warning", "Invalid character detected! don't type '`'");
            return;
        }
        else if (!nameEdit.empty())
        {
            options.push_back(nameEdit);
        }
        else
        {
            QMessageBox::warning(nullptr, "Warning", "Empty option detected!");
            return;
        }
    }

    if (pollName.empty() || voterId.empty() || !options.size())
    {
        QMessageBox::warning(nullptr, "Warning", "Please provide a title, description, voter ID, and add some options!");
        return;
    }

    CreatePoll newPoll
        {
            voterId,
            pollName,
            pollDesc,
            activeUser.id,
            options
        };

    create_poll(newPoll);
    QMessageBox::information(nullptr, "Success", "Poll created successfully!");
}
