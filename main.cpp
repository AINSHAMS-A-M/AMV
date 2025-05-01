#include "mainwindow.h"
#include <QApplication>
#include "db.hpp"

int main(int argc, char *argv[])
{
    QApplication ab(argc, argv);
    load_data();
    MainWindow w;
    w.show();
    return ab.exec();
}
