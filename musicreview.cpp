#include "musicreview.h"
#include "ui_musicreview.h"
#include "mainwindow.h"
#include "musicinfo.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QMessageBox>
MusicReview::MusicReview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicReview)
{
    ui->setupUi(this);
}

MusicReview::~MusicReview()
{
    delete ui;
}

void MusicReview::receive(QString s[3]){
    QFile file("D:/UC Dylan/3º ano 1º semestre/BD/Project/BasesDados/username.txt");
    file.open(QFile::ReadOnly);

    // Get username from file
    QTextStream in(&file);
    QString username = in.readAll();
    file.close();
    ui->user->setText(username);
    ui->music->setText(s[0]);
    ui->artist->setText(s[1]);
    param[0] = s[0];
    param[1] = s[1];
}

void MusicReview::on_ArtistB_4_clicked()
{
    QFile file("D:/UC Dylan/3º ano 1º semestre/BD/Project/BasesDados/username.txt");
    file.open(QFile::ReadOnly);

    // Get username from file
    QTextStream in(&file);
    QString username = in.readAll();
    file.close();
    ui->user->setText(username);

    QString score = ui->score->toPlainText();
    QString review = ui->review->toPlainText();
    QSqlQuery query;

    query.prepare("select m.id from musica m, musica_artista, artista a where musica_id = id and artista_nome = a.nome and datainicio = artista_datainicio and m.nome = :nome and a.nome = :artista;");
    query.bindValue(":nome", param[0]);
    query.bindValue(":artista", param[1]);
    query.exec();
    query.next();

    int id = query.value(0).toInt();


    //qDebug() << data << query.value(0).toString();
    query.prepare("INSERT INTO criticamusica(musica_id, critica_data, critica_pontuacao, critica_justificacao, critica_utilizador_nick) VALUES(:id, :data, :pontuacao, :just, :nick);");
    query.bindValue(":id", id);
    query.bindValue(":data", 2018-11-28);
    query.bindValue(":pontuacao", score.toInt());
    query.bindValue("just", review);
    query.bindValue(":nick", username);
    query.exec();
    query.next();

    QMessageBox::information(this, tr("Sucess."),
                             tr("Review posted with sucess."));
    this->hide();
}

void MusicReview::on_GoBackB_4_clicked()
{
    this->hide();
}


