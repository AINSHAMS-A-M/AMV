#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include "db.hpp"

int main(int argc, char *argv[])
{
    QApplication ab(argc, argv);
    load_data();
    MainWindow w;
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    w.showFullScreen();
    w.setWindowIcon(QIcon(":/logo.png"));
    QGuiApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    qApp->setPalette(palette);
    w.show();
    return ab.exec();
}
