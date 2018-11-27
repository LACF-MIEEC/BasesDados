#include "musicinfo.h"
#include "ui_musicinfo.h"
#include "mainwindow.h"
#include "startwindow.h"
#include "search.h"
#include "musicreview.h"


#include <QDebug>
#include <QThread>
MusicInfo::MusicInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicInfo)
{

    ui->setupUi(this);


}

MusicInfo::~MusicInfo()
{
    delete ui;
}

void MusicInfo::on_MainB_clicked()
{
    MainWindow sw;
    this->hide();
    sw.setModal(true);
    sw.exec();
}

void MusicInfo::on_GoBackB_clicked()
{
    Search sw;
    this->hide();
    sw.setModal(true);
    sw.exec();
}

void MusicInfo::receive(QString s[3]){
    valm[0] = s[0];
    valm[1] = s[1];
    valm[2] = s[2];

    StartWindow conn;
    QSqlQueryModel * modal1 = new QSqlQueryModel();
    QSqlQueryModel * modal2 = new QSqlQueryModel();
    QSqlQueryModel * modal3 = new QSqlQueryModel();
    QSqlQueryModel * modal4 = new QSqlQueryModel();
    QSqlQueryModel * modal5 = new QSqlQueryModel();
    QSqlQueryModel * modal6 = new QSqlQueryModel();
    QSqlQueryModel * modal7 = new QSqlQueryModel();
    QSqlQueryModel * modal8 = new QSqlQueryModel();
    QSqlQuery* query = new QSqlQuery(conn.db);

    // Music name
    query->prepare("select m.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal1->setQuery(*query);
    ui->name->setModel(modal1);

    // Music artist
    query->prepare("select ar.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal2->setQuery(*query);
    ui->artist->setModel(modal2);

    // Music album
    query->prepare("select al.nome from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal3->setQuery(*query);
    ui->album->setModel(modal3);

    // Music data
    query->prepare("select m.data from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal4->setQuery(*query);
    ui->data->setModel(modal4);

    // Music duration
    query->prepare("select m.duracao from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal5->setQuery(*query);
    ui->duration->setModel(modal5);

    // Lyrics
    query->prepare("select m.letra from musica m, album al, artista ar, musica_album mal, musica_artista mar where m.id = mal.musica_id and al.id = mal.album_id and m.id = mar.musica_id and ar.nome = mar.artista_nome and mar.artista_datainicio = ar.datainicio and m.nome = :music and al.nome = :album and ar.nome = :artist;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal6->setQuery(*query);
    ui->Lyrics->setModel(modal6);

    // Genre
    query->prepare("select tipo from genero, musica, musica_genero where id = musica_id and genero_tipo = tipo;");
    query->bindValue(":music", valm[0]);
    query->bindValue(":artist", valm[1]);
    query->bindValue(":album", valm[2]);
    query->exec();

    modal7->setQuery(*query);
    ui->genre->setModel(modal7);

    // Reviews
    query->prepare("select m.id from musica m, musica_artista, artista a where musica_id = id and artista_nome = a.nome and datainicio = artista_datainicio and m.nome = :nome and a.nome = :artista;");
    query->bindValue(":nome", valm[0]);
    query->bindValue(":artista", valm[1]);
    query->exec();
    query->next();

    int id = query->value(0).toInt();

    query->prepare("select critica_utilizador_nick, critica_pontuacao from criticamusica where musica_id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->next();
    modal8->setQuery(*query);
    ui->Reviews->setModel(modal8);
}

// artist info
void MusicInfo::on_ArtistB_clicked()
{

}

// Music review
void MusicInfo::on_MainB_2_clicked()
{
    QFile file("D:/UC Dylan/3º ano 1º semestre/BD/Project/BasesDados/username.txt");
    file.open(QFile::ReadOnly);

    // Get username from file
    QTextStream in(&file);
    QString username = in.readAll();
    file.close();

    QSqlQuery query;
    query.prepare("Select count(*) from criticamusica where critica_utilizador_nick = :user;");
    query.bindValue(":user", username);
    query.next();
    query.exec();
    if(query.value(0).toInt() == 1){
        QMessageBox::information(this, tr("Error."),
                                 tr("That user has already done a review for this music."));
        return;
    }


    MusicReview sw;
    connect(this, &MusicInfo::send_values, &sw, &MusicReview::receive);
    emit send_values(valm);
    sw.setModal(true);
    sw.exec();
}
