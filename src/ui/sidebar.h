#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "QBoxLayout"
#include <QWidget>
#include <QStringList>
#include <QPushButton>

class SidebarWidget : public QWidget {
    Q_OBJECT

public:
    explicit SidebarWidget(QWidget *parent, std::string page);

private:
    void setupMenuItems(std::string page);
    void setupWelcomeLabel();
    void setupLogoutButton();
    QVBoxLayout* sbLayout;
};


#endif
