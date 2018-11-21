#include "mainwindow.h"
#include "startwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>

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
