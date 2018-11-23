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


}

Register::~Register()
{
    delete ui;
}

void Register::on_Cancel_clicked() // CANCEL BUTTON
{
    // Go back to start menu
    this->hide();
    StartWindow start;
    start.setModal(true);
    start.exec();

}

void Register::on_Register_2_clicked()
{
        QString nick = ui->nick->text();
        QString password = ui->password->text();
        QString email = ui->email->text();
        QString name = ui->name->text();
        QString lname = ui->lastname->text();

        if(nick.isEmpty() || password.isEmpty() || email.isEmpty() || name.isEmpty()
                || lname.isEmpty()){
            QMessageBox::information(this, tr("Invalid entries"),
                                     tr("All fields are required!"));
            // keep in window
            return;
        }

        QSqlQuery query;

        // Check if user already exists
        query.prepare("SELECT count(*) FROM utilizador WHERE nick = :username");
        query.bindValue(":username", nick);
        query.exec();
        query.next();
        if(query.value(0).toInt() == 1){
            QMessageBox::information(this, tr("Invalid User"),
                                     tr("User already exist!"));
            return;
        }

        // Check if email is already token
        query.prepare("SELECT count(*) FROM utilizador WHERE email = :email");
        query.bindValue(":email", email);
        query.exec();
        query.next();
        if(query.value(0).toInt() == 1){
            QMessageBox::information(this, tr("Invalid email"),
                                     tr("This email has already an account!"));
            return;
        }

        // Insert new user
        query.prepare("INSERT INTO utilizador (nick, password, ultimonome, primeironome, email, dataregisto, gestor, mailconfirmado) Values (:nick, :password, :ultimonome, :primeironome, :email, '2018-11-23', false, false)");
        query.bindValue(":nick", nick);
        query.bindValue(":password", password);
        query.bindValue(":ultimonome", lname);
        query.bindValue(":primeironome", name);
        query.bindValue(":email", email);
        query.exec();
        query.next();

        QMessageBox::information(this, tr("Sucess!"),
                                     tr("Welcome to the Jungle!!!")); // Change?

        // Go back to start menu
        this->hide();
        StartWindow start;
        start.setModal(true);
        start.exec();
}


