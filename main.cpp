#include "mainwindow.h"
#include <QApplication>
#include "db.hpp"

int main(int argc, char *argv[])
{
    QApplication ab(argc, argv);
    load_data();
    MainWindow w;
    w.showMaximized(); // Make fullscreen later
    w.setWindowIcon(QIcon(":/logo.png"));
    w.show();
    return ab.exec();
}
