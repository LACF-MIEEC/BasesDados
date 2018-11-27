#include "startwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartWindow w;
    w.setModal(true);
    w.exec();

    return a.exec();
}
