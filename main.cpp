#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <algorithm>

#include "services.hpp"
#include "data_structures.hpp"
#include "db.hpp"

int main(int argc, char *argv[])
{
    QApplication ab(argc, argv);
    load_data();
    MainWindow w;
    w.show();
    return ab.exec();
}
