#include "mainwindow.h"
#include "myprofile.h"
#include "ui_mainwindow.h"
#include "search.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // BackGround Image
    QPixmap pix("D:/UC Dylan/3º ano 1º semestre/BD/Project/BasesDados/resources/logos/main_menu.png");
    ui->BackGround->setPixmap(pix.scaled(1000,519,Qt::KeepAspectRatio));
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
