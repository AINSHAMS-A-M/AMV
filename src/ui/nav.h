
#ifndef NAV_H
#define NAV_H

#include <QObject>

class NavigationManager : public QObject {
    Q_OBJECT
public:
    enum Page { Login, Register, Help, Vote, CreatePoll, MyPolls, MyVotes, Profile, Logout };
    static NavigationManager& instance();
    void navigate(Page page) { emit navigateTo(page); }

signals:
    void navigateTo(Page page);

private:
    NavigationManager(QObject* parent = nullptr);
};

#endif
