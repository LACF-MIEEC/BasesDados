#include "mainwindow.h"
#include "myprofile.h"
#include "ui_mainwindow.h"
#include "search.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_MyProfile_clicked()
{
    this->hide();
    MyProfile sw;
    sw.setModal(true);
    sw.exec();
}


void MainWindow::on_Search_clicked()
{
    this->hide();
    Search search;
    search.setModal(true);
    search.exec();
}
