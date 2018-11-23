#include "register.h"
#include "ui_register.h"
#include "startwindow.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);



    StartWindow *sw = new StartWindow(this);
    connect(sw,SIGNAL(accepted()),this,SLOT(loggedIn()));
    //Read Remember File
    sw->show();


}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_clicked() // CANCEL BUTTON
{
    // Voltar ao menu de start

}

void Register::RegClick(){
    this->show();
    return;
}

void Register::on_pushButton_2_clicked()
{

}
