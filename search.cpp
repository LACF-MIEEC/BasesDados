#include "search.h"
#include "ui_search.h"
#include "startwindow.h"

#include <QSqlDatabase>
#include <QtWidgets>
#include <QtSql>
#include <QMessageBox>

Search::Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
}

Search::~Search()
{
    delete ui;
}

void Search::on_SearchB_clicked()
{
    QString search = ui->lineS->text();

    if (search.isEmpty()){
        QMessageBox::information(this, tr("Invalid search!"),
                                 tr("Search field is empty."));
        return;
    }

    StartWindow conn;
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery* query = new QSqlQuery(conn.db);

    //conn.connOpen();
    // Select from BD
    query->prepare("SELECT * FROM musica WHERE nome = :music ");
    query->bindValue(":music", search);
    query->exec();

    /*if(query.value(0).toInt() == 0){
        QMessageBox::information(this, tr("Not found!"),
                                 tr("Music not found."));
        return;
    }*/
    modal->setQuery(*query);
    ui->table->setModel(modal);

    qDebug() <<(modal->rowCount());


    return;
}
