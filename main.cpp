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
    MainWindow w;
    w.show();
    std::cout << "WELCOME TO AMV\n";
    MeshVector<int> a;
    a.push_back(2);
    a.push_back(4);
    a.push_back(5);
    for (auto x : a)
        std::cout << x << ' ';
    return ab.exec();
}
