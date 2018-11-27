#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>
#include <QtSql>
#include "register.h"
#include <QMessageBox>


namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();
    QSqlDatabase db;
    void connClose(){
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    void connOpen(){
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
        db = QSqlDatabase::addDatabase(DBdriver);
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


private slots:
    void on_LogInButton_clicked();

    void on_RegisterButton_clicked();

    void on_DBSettings_clicked();

signals:
    void send_user(QString user);
private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
