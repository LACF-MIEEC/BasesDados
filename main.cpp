#include "mainwindow.h"

#include <QApplication>

#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    return a.exec();
}
