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
    // if not available use default
    DBdriver  = "QPSQL";
    DBname    = "postgres";
    DBhost    = "localhost";
    DBport    = 5432;
    DBuser    = "postgres";
    DBpasswd  = "postgres";

    QFile file("DatabaseSettings.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        QTextStream in(&file);
        QStringList settings;

        while (!in.atEnd()) {
            settings.append(in.readLine());
        }
        if(settings.size() == 5){

            DBdriver  = settings.at(0);
            DBname    = settings.at(1);
            DBhost    = settings.at(2);
            DBport    = settings.at(3).toInt();
            DBuser    = settings.at(4);
            DBpasswd  = settings.at(5);
        }
        file.close();
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

    currentUser.clear();
    this->setHidden(true);

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

    ui->stackedWidget->setCurrentWidget(ui->homePage);

    // hide search widgets
    ui->details->hide();
    ui->interGroupBox->hide();
    ui->musicsGroupBox->hide();
    ui->albumsGroupBox->hide();
    ui->concertsGroupBox->hide();
    ui->playlistsGroupBox->hide();

    this->show();

    this->findChild<QDialog*>("StartWindow")->deleteLater();

    currentUser = username;


    //////////////////////////////////////////////////////////////////////
    /// Write Log Register
    ///
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    QDateTime timestamp;
    // Get Timestamp
    query.prepare("SELECT current_timestamp");
    query.exec();
    query.first();
    timestamp = query.value(0).toDateTime();
    qDebug() << "Login Date: " << timestamp;

    if(query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES (:data, :tipoacesso, :utilizador_nick)")){
        query.bindValue(":data", timestamp);
        query.bindValue(":tipoacesso", "LogIn");
        query.bindValue(":utilizador_nick", username);

        if(!query.exec()){
            qDebug() << "ERROR: " << query.lastError();
        }
        qDebug() << "Inserted " << query.numRowsAffected() << " into registoacesso";
    }else
        qDebug() << "ERROR: " << query.lastError();

    refreshAll();

    query.finish();
    query.clear();
    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!currentUser.isEmpty()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Exit Music Database"),
                                                                    tr("Are you sure?\n"));
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
            return;
        } else {
            {
                QSqlDatabase db = QSqlDatabase::database();
                QSqlQuery query(db);
                QDateTime timestamp;
                // Get Timestamp
                query.prepare("SELECT current_timestamp");
                query.exec();
                query.first();
                timestamp = query.value(0).toDateTime();
                qDebug() << "Logoff Date: " << timestamp;

                if(query.prepare("INSERT INTO registoacesso (data, tipoacesso, utilizador_nick) VALUES (:data, :tipoacesso, :utilizador_nick)")){
                    query.bindValue(":data", timestamp);
                    query.bindValue(":tipoacesso", "LogOff");
                    query.bindValue(":utilizador_nick", currentUser);

                    if(!query.exec()){
                        qDebug() << "ERROR: " << query.lastError();

                    }
                    qDebug() << "Inserted " << query.numRowsAffected() << " into registoacesso";
                }else
                    qDebug() << "ERROR: " << query.lastError();

                query.finish();
                query.clear();
                db.close();
            }
            QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        }
    }

    event->accept();
}

void MainWindow::refreshAll(){

    refreshUserPanel();

}

void MainWindow::refreshUserPanel(){

    ui->UserInfoBox->setTitle(currentUser);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    query.prepare("SELECT primeironome, ultimonome, email, pais, bio, imagem FROM utilizador WHERE nick=:username");
    query.bindValue(":username",currentUser);
    query.exec();
    query.first();
    ui->Name->setText(query.value(0).toString() + " " + query.value(1).toString());
    ui->email->setText(query.value(2).toString());
    ui->country->setText(query.value(3).toString());
    ui->bio->setText(query.value(4).toString());
    if(!query.value(5).isNull()){
        QByteArray outByteArray = query.value(5).toByteArray();
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(outByteArray);

        ui->UserPic->setPixmap(outPixmap);
    }
    query.finish();
    query.clear();

}

void MainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->homePage);
}

void MainWindow::on_filesButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->uploadsPage);
}

void MainWindow::on_searchButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->searchPage);

    QStringList searchWords = ui->searchBar->text().split(" ", QString::SkipEmptyParts);

    //PERFORM SEARCH FOR RELATED WORDS
}

void MainWindow::on_searchBar_returnPressed()
{
    on_searchButton_clicked();
}
