#include "startwindow.h"
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

    this->setWindowFlag(Qt::SplashScreen);

    QFile file("UserCredentials.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QStringList cred;

        while (!in.atEnd()) {
            cred.append(in.readLine());
        }

        if(cred.size() == 2){

            ui->UserNameEdit->setText(cred.at(0));
            ui->PasswordEdit->setText(cred.at(1));

            ui->RememberCheck->setChecked(true);
        }
        file.close();
    }

}

StartWindow::~StartWindow()
{
    delete ui;

}

void StartWindow::on_LogInButton_clicked()
{
    // Check for username and password
    QString username = userName();
    QString password = passWord();
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


    if(saveCredentialsSet()){
        QFile file("UserCredentials.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << username << "\n" << password;
            file.close();
        }

    } else {
        QFile file("UserCredentials.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.close();
        }
    }

    query.finish();
    query.clear();

    this->acceptedUser(username);
    this->accept();
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

    QFile file("DatabaseSettings.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){


            QTextStream out(&file);
            out << dialog.driverName() << "\n"
                << dialog.databaseName() << "\n"
                << dialog.hostName() << "\n"
                << dialog.port() << "\n"
                << dialog.userName() << "\n"
                << dialog.password();

            file.close();
    }


}

QString StartWindow::userName() const
{
    return ui->UserNameEdit->text();
}
QString StartWindow::passWord() const
{
    return ui->PasswordEdit->text();
}
bool StartWindow::saveCredentialsSet() const
{
    return ui->RememberCheck->isChecked();
}
