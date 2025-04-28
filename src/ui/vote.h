#pragma once

#include <QWidget> // Base class
#include <QStringList> // Needed for menu items

QT_BEGIN_NAMESPACE
class QLabel;
class QTextEdit;
class QWidget;
class QPushButton; // Include QPushButton forward declaration
QT_END_NAMESPACE

class VotePage : public QWidget
{
    Q_OBJECT

public:
    // Constructor declaration only
    explicit VotePage(QWidget *parent = nullptr);
    ~VotePage() override = default;

signals:
    // Signals to indicate which sidebar menu item was clicked
    void onHelpClicked();
    void onVoteClicked();
    void onMyVotesClicked();
    void onCreatePollClicked();
    void onMyPollsClicked();
    void onProfileClicked();
private:
    QWidget    *sidebar;
    QWidget    *content;

    QList<QPushButton*> sidebarButtons;

};
