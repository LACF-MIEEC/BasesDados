#include "startwindow.h"
#include "mainwindow.h"
#include "register.h"
#include "ui_startwindow.h"
#include "qsqlconnectiondialog.h"


#include <QMessageBox>
#include <QSqlQuery>
#include <QtSql>

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    // Connect to data base
    if (QSqlDatabase::drivers().isEmpty())
        QMessageBox::information(this, tr("No database drivers found"),
                                 tr("This program requires at least one Qt database driver. "
                                    "Please check the documentation how to build the "
                                    "Qt SQL plugins."));
    // Load setting from file
    // if not use default
    QString DBdriver  = "QPSQL";
    QString DBname    = "postgres";
    QString DBhost    = "localhost";
    int DBport        = 5432;
    QString DBuser    = "postgres";
    QString DBpasswd  = "postgres";


    //Start Database Connection
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(DBdriver);
    db.setDatabaseName(DBname);
    db.setHostName(DBhost);
    db.setPort(DBport);

    if (!db.open(DBuser, DBpasswd)) {
        err = db.lastError();
        QSqlDatabase::removeDatabase(db.connectionName());
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                       "opening the connection: ") + err.text());
    }

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_LogInButton_clicked()
{
    // Check for username and password
    QString username = ui->UserNameEdit->text();
    QString password = ui->PasswordEdit->text();
    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::information(this, tr("User and Password Required"),
                                 tr("To Login you must provide a username and a password!"));
        // keep in window
        return;
    }
    QSqlQuery query;

    // Check User
    query.prepare("SELECT count(*) FROM utilizador WHERE nick = :username");
    query.bindValue(":username", username);
    query.exec();
    query.next();
    if(query.value(0).toInt() == 0){
        QMessageBox::information(this, tr("Invalid User"),
                                 tr("To Login you must provide an existing user!"));
        return;
    }

    // Check Password
    query.prepare("SELECT password FROM utilizador WHERE nick = :username");
    query.bindValue(":username", username);
    query.exec();
    query.next();
    if(password != query.value(0).toString()){
        QMessageBox::information(this, tr("Wrong Password"),
                                 tr("The password provided is invalid!"));
        return;
    }


    if(ui->RememberCheck->isChecked()){
        //Save Credentials into file?
    }

    MainWindow main;
    main.show();
}

void StartWindow::on_RegisterButton_clicked()
{
    // Create Register Form
    // Submit to database
    // if submition OK
    // promt OK dialog
    // else
    // promt error dialog
    // Go back to LogIn window
    Register reg;
    this->hide();
    reg.setModal(true);
    reg.exec();
}

void StartWindow::on_DBSettings_clicked()
{
    QSqlConnectionDialog dialog(this);

    // TODO: Load settings from
    if (dialog.exec() != QDialog::Accepted)
        return;

    //Start Database Connection
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(dialog.driverName());
    db.setDatabaseName(dialog.databaseName());
    db.setHostName(dialog.hostName());
    db.setPort(dialog.port());
    if (!db.open(dialog.userName(), dialog.password())) {
        err = db.lastError();
        QSqlDatabase::removeDatabase(db.connectionName());
        if (err.type() != QSqlError::NoError)
            QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                       "opening the connection: ") + err.text());
    }
    // Save setting to file


}
