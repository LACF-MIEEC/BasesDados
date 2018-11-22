#include "mainwindow.h"
#include "startwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (QSqlDatabase::drivers().isEmpty())
        QMessageBox::information(this, tr("No database drivers found"),
                                 tr("This program requires at least one Qt database driver. "
                                    "Please check the documentation how to build the "
                                    "Qt SQL plugins."));
    QString DBdriver;
    QString DBname;
    QString DBhost;
    int DBport;
    QString DBuser;
    QString DBpasswd;

    // Load setting from file
    QFile file("DatabaseSettings.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        // if not available use default
        DBdriver  = "QPSQL";
        DBname    = "postgres";
        DBhost    = "localhost";
        DBport    = 5432;
        DBuser    = "postgres";
        DBpasswd  = "postgres";
    }
    else{

        QTextStream in(&file);
        QStringList settings;

        while (!in.atEnd()) {
            settings.append(in.readLine());
        }

        DBdriver  = settings.at(0);
        DBname    = settings.at(1);
        DBhost    = settings.at(2);
        DBport    = settings.at(3).toInt();
        DBuser    = settings.at(4);
        DBpasswd  = settings.at(5);
    }


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

    StartWindow *sw = new StartWindow(this);
    connect(sw,SIGNAL(acceptedUser(QString)),this,SLOT(loggedIn(QString)));
    connect(sw, SIGNAL(rejected()),this,SLOT(close()));
    //Read Remember File
    sw->open();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loggedIn(QString username){
    this->show();

    currentUser = username;

    QSqlQuery query;
    QDateTime timestamp;
    // Get Timestamp
    query.prepare("SELECT current_timestamp");
    query.exec();
    query.next();
    timestamp = query.value(0).toDateTime();
    qDebug() << "Login Date: " << timestamp;

    query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES (:data, :tipoacesso, :utilizador_nick)");
    query.bindValue(":data", timestamp);
    query.bindValue(":tipoacesso", "LogIn");
    query.bindValue(":utilizador_nick", username);

    if(!query.exec()){
        qDebug() << "ERROR: " << query.lastError();
    }

    query.finish();
    query.clear();
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exit Music Database"),
                                                                tr("Are you sure?\n"));
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {

        QSqlQuery query;
        QDateTime timestamp;
        // Get Timestamp
        query.prepare("SELECT current_timestamp");
        query.exec();
        query.next();
        timestamp = query.value(0).toDateTime();
        qDebug() << "Logoff Date: " << timestamp;

        query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES (:data, :tipoacesso, :utilizador_nick)");
        query.bindValue(":data", timestamp);
        query.bindValue(":tipoacesso", "LogOff");
        query.bindValue(":utilizador_nick", currentUser);

        if(!query.exec()){
            qDebug() << "ERROR: " << query.lastError();
        }

        query.finish();
        query.clear();

        QSqlDatabase::database().close();
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());

        event->accept();
    }
}
