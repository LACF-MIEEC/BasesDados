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

    ui->stackedWidget->setCurrentIndex(0);

    this->setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);

    QFile file(":/Files/Countries");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QStringList countries;

        while (!in.atEnd()) {
            countries.append(in.readLine());
        }
        ui->country->addItems(countries);
    }
    file.close();
    file.setFileName("UserCredentials.txt");
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
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // Check User
    if(query.prepare("SELECT count(*) FROM utilizador WHERE nick = :username")){
        query.bindValue(":username", username);
        query.exec();
        query.first();
        if(query.value(0).toInt() == 0){
            QMessageBox::information(this, tr("Invalid User"),
                                     tr("To Login you must provide an existing user!"));
            query.finish();
            query.clear();
            return;
        }
    }else
        qDebug() << "LOGIN ERROR: " << query.lastError() << " at getting if username exists";

    // Check Password
    if(query.prepare("SELECT password FROM utilizador WHERE nick = :username")){
        query.bindValue(":username", username);
        query.exec();
        query.first();
        if(password != query.value(0).toString()){
            QMessageBox::information(this, tr("Wrong Password"),
                                     tr("The password provided is invalid!"));
            query.finish();
            query.clear();
            return;
        }
    }else
        qDebug() << "LOGIN ERROR: " << query.lastError() << " at getting if password valid";

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
    ui->stackedWidget->setCurrentIndex(1);
}

void StartWindow::on_DBSettings_clicked()
{
    QSqlConnectionDialog dialog(this);
    this->setHidden(true);

    if (dialog.exec() != QDialog::Accepted){
        this->setHidden(false);
        return;
    }

    this->setHidden(false);
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

void StartWindow::on_RegButtonBox_rejected()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartWindow::on_RegButtonBox_accepted()
{
    QString nickname = ui->nick->text();
    QString password = ui->password->text();
    QString firstname = ui->firstName->text();
    QString lastname = ui->lastName->text();
    QString email    = ui->email->text();

    if( nickname.isEmpty() || password.isEmpty() || firstname.isEmpty() || lastname.isEmpty() || email.isEmpty() ){
        QMessageBox::information(this, tr("Blank Mandatory fields!"),
                                 tr("All the mandatory fields(*) must be given!"));
        return;
    }

    if( QString::compare(password,ui->confirmPassword->text()) != 0 )
    {
        QMessageBox::information(this, tr("Passwords doesn't match!"),
                                 tr("Password must be the same as confirm password."));
        return;
    }

    if( !email.contains('@') ){
        QMessageBox::information(this, tr("Invalid e-mail!"),
                                 tr("Please provide a valid e-mail address!"));
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    // Check User
    if(query.prepare("SELECT count(*) FROM utilizador WHERE nick = :username")){
        query.bindValue(":username", nickname);
        query.exec();
        query.first();
        if(query.value(0).toInt() != 0){
            QMessageBox::information(this, tr("Invalid Nick"),
                                     tr("The nickname you provided already exists!"));
            query.finish();
            query.clear();
            return;
        }
    }else
        qDebug() << "REGISTER ERROR: " << query.lastError() << " at getting if username exists";
    query.finish();

    // Check Email
    if(query.prepare("SELECT count(*) FROM utilizador WHERE email = :email")){
        query.bindValue(":email", email);
        query.exec();
        query.first();
        if(query.value(0).toInt() != 0){
            QMessageBox::information(this, tr("Invalid e-mail!"),
                                     tr("The e-mail you provided is already registred!"));
            query.finish();
            query.clear();
            return;
        }
    }else
        qDebug() << "REGISTER ERROR: " << query.lastError() << " at getting if email exists";
    query.finish();

    QString bio = ui->bio->toPlainText();
    QString country = ui->country->currentText();
    QDate birthDate = ui->birthDate->date();
    QDateTime sysdate;

    // Get timestamp
    query.prepare("SELECT current_timestamp");
    query.exec();
    query.first();
    sysdate = query.value(0).toDateTime();
    query.finish();
    query.clear();

    if(query.prepare("INSERT INTO utilizador (nick, password, primeironome, ultimonome, email, gestor, mailconfirmado, datanascimento, bio, pais, dataregisto)"
                     "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")){
        query.addBindValue(nickname);
        query.addBindValue(password);
        query.addBindValue(firstname);
        query.addBindValue(lastname);
        query.addBindValue(email);
        query.addBindValue(false);
        query.addBindValue(false);
        query.addBindValue(birthDate);
        query.addBindValue(bio);
        query.addBindValue(country);
        query.addBindValue(sysdate);
        if(!query.exec()){
            qDebug() << "ERROR: " << query.lastError();
            QMessageBox::warning(this, tr("Error"),
                                 tr("Something went Wrong!"));

        }
    }else
        qDebug() << "REGISTER ERROR: " << query.lastError() << "  at Inserting data";

    query.finish();
    query.clear();
    QMessageBox::information(this, tr("WELCOME!!"),
                             tr("You have succesfully registred into Music DataBase!\n Enjoy your Music!!"));

    this->acceptedUser(nickname);
    this->accept();

}

void StartWindow::on_cancelButton_clicked()
{
    this->reject();
}
