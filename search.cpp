#include "search.h"
#include "ui_search.h"
#include "startwindow.h"
#include "mainwindow.h"
#include "musicinfo.h"

#include <QSqlDatabase>
#include <QtWidgets>
#include <QtSql>
#include <QMessageBox>
#include <QThread>

// Global variables on this frame


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
    bool mus = ui->musicCh->checkState();
    bool alb = ui->albumCh->checkState();
    bool art = ui->artistCh->checkState();


    if (search.isEmpty()){
        QMessageBox::information(this, tr("Invalid search!"),
                                 tr("Search field is empty."));
        return;
    }

    if ((mus==1 && art==1) || (mus==1 && alb==1)|| (art==1 && alb==1) || (mus==1 && alb==1 && art==1)){
        QMessageBox::information(this, tr("Invalid entries!"),
                                 tr("Please only check one CheckBox."));
        return;
    }
    StartWindow conn;
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery* query = new QSqlQuery(conn.db);

    if(mus==1){
        // Select from BD
        query->prepare("select m.nome, ar.nome, al.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music;");
        query->bindValue(":music", search);
        query->exec();

        modal->setQuery(*query);
        ui->table->setModel(modal);
    }
    else if(alb==1){
        // Select from BD
        query->prepare("select m.nome, ar.nome, al.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and al.nome = :album;");
        query->bindValue(":album", search);
        query->exec();

        modal->setQuery(*query);
        ui->table->setModel(modal);
    }
    else if(art==1){
        // Select from BD
        query->prepare("select m.nome, ar.nome, al.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and ar.nome = :artist;");
        query->bindValue(":artist", search);
        query->exec();

        modal->setQuery(*query);
        ui->table->setModel(modal);
    }


    return;
}

void Search::on_table_doubleClicked(const QModelIndex &index)
{
    MusicInfo sw;


    // Double click on 1st column
    if (index.column()==0){
        val[0] = ui->table->model()->data(index).toString();
        QModelIndex index1 = index.sibling(index.row(), index.column()+1);
        val[1] = ui->table->model()->data(index1).toString();
        QModelIndex index2 = index.sibling(index.row(), index.column()+2);
        val[2] = ui->table->model()->data(index2).toString();

        // connect with 'MusicInfo' frame
        connect(this, &Search::sendParams, &sw, &MusicInfo::receive);
        emit sendParams(val);

        this->hide();
        sw.setModal(true);
        sw.exec();
    }
    // Double click on 2nd column
    else if(index.column()==1){
        val[1] = ui->table->model()->data(index).toString();
        QModelIndex index1 = index.sibling(index.row(), index.column()+1);
        val[2] = ui->table->model()->data(index1).toString();
        QModelIndex index2 = index.sibling(index.row(), index.column()-1);
        val[0] = ui->table->model()->data(index2).toString();
        connect(this, &Search::sendParams, &sw, &MusicInfo::receive);
        emit sendParams(val);
        this->hide();
        sw.setModal(true);
        sw.exec();
    }
    // Double click on 3rd column
    else if(index.column()==2){
        val[2] = ui->table->model()->data(index).toString();
        QModelIndex index1 = index.sibling(index.row(), index.column()-1);
        val[1] = ui->table->model()->data(index1).toString();
        QModelIndex index2 = index.sibling(index.row(), index.column()-2);
        val[0] = ui->table->model()->data(index2).toString();
        connect(this, &Search::sendParams, &sw, &MusicInfo::receive);
        emit sendParams(val);
        this->hide();
        sw.setModal(true);
        sw.exec();
    }
}

void Search::on_pushButton_clicked()
{
    MainWindow sw;
    this->hide();
    sw.setModal(true);
    sw.exec();

}
