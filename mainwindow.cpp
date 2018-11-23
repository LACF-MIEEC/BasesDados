#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startwindow.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    StartWindow *sw = new StartWindow(this);
    connect(sw,SIGNAL(accepted()),this,SLOT(loggedIn()));
    //Read Remember File
    sw->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loggedIn(){
    this->show();
    return;
}
