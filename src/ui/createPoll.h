#pragma once

#include "sidebar.h"
#include <QWidget>
#include <QScrollArea>
#include <QStringList>
#include <QList>
#include <QLineEdit>
#include <QTextEdit>
#include <QVector>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QFormLayout;
QT_END_NAMESPACE

class CreatePollPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreatePollPage(QWidget *parent = nullptr);
    ~CreatePollPage() override = default;

signals:
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();

public slots:
    void onAddOptionClicked();
    void onRemoveOptionClicked();
    void onCreatePollBtnClicked();

private:
    void addOptionWidget();

    SidebarWidget *sidebar;
    QWidget               *content;
    QScrollArea           *optionsScrollArea;
    QList<QPushButton*>    sidebarButtons;
    QLineEdit             *pollNameEdit;
    QTextEdit             *pollDescEdit;
    QLineEdit             *voterIdEdit;
    QWidget               *optionsContainer;
    QVBoxLayout           *optionsLayout;
    QVector<QWidget*>      optionWidgets;
};
