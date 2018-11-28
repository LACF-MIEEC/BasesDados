#include "playlisteditor.h"
#include "ui_playlisteditor.h"

#include <QSqlRelationalTableModel>
#include <QtSql>
#include <QMessageBox>


PlayListEditor::PlayListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayListEditor)
{
    ui->setupUi(this);

    QTableView *view = ui->allMusicsTableView;
    view->setModel(new QSqlQueryModel(view));
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setShowGrid(false);
    view->setGridStyle(Qt::NoPen);
    view->setSortingEnabled(false);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setCascadingSectionResizes(true);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    this->setWindowFlags(Qt::SplashScreen);

    QSqlDatabase::database().transaction();
}

PlayListEditor::~PlayListEditor()
{
    delete ui;
}

void PlayListEditor::on_buttonBox_2_accepted()
{
    if(ui->selectedMusics->model()->rowCount() == 0){
        QMessageBox::information(this,"Insufficient Musics","At least one music is required!");
    }
    else{
        QSqlQuery query(QSqlDatabase::database());
        QSqlRelationalTableModel *playlistModel = qobject_cast<QSqlRelationalTableModel*>(ui->selectedMusics->model());

        query.prepare(QString("UPDATE playlist SET descricao = '%1' WHERE nome = '%2' AND utilizador_nick = '%3'").arg(ui->description->toPlainText()).arg(ui->name->text()).arg(currentUser));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            emit failure();
            this->reject();
            return;
        }
        query.prepare(QString("UPDATE playlist SET private = '%1' WHERE nome = '%2' AND utilizador_nick = '%3'").arg(ui->setPrivate->isChecked()).arg(ui->name->text()).arg(currentUser));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            emit failure();
            this->reject();
            return;
        }
        if(!playlistModel->submitAll()){
            QSqlDatabase::database().rollback();
            emit failure();
            this->reject();
            return;
        }

        QSqlDatabase::database().commit();
        this->accept();
    }
}

void PlayListEditor::on_buttonBox_2_rejected()
{
    QSqlDatabase::database().rollback();
    this->reject();
}

void PlayListEditor::setAllMusics(QSqlQuery * query){
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->allMusicsTableView->model());
    model->setQuery(*query);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Album"));
    model->setHeaderData(3, Qt::Horizontal, tr("Interpreter"));
    ui->allMusicsTableView->setColumnHidden(0, true);
}

void PlayListEditor::configureSelectedMusics(){

    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(ui->selectedMusics, QSqlDatabase::database());
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable(QSqlDatabase::database().driver()->escapeIdentifier("playlist_musica", QSqlDriver::TableName));
    model->setFilter(QString("playlist_utilizador_nick = '%1' AND playlist_nome = '%2'").arg(currentUser).arg(ui->name->text()));
    model->select();
    if (model->lastError().type() != QSqlError::NoError)
        emit failure();


    model->setHeaderData(2, Qt::Horizontal, "Music Name");
    model->setRelation(2,QSqlRelation("musica","id","nome"));
    QTableView *view = ui->selectedMusics;

    view->setModel(model);
    view->setColumnHidden(0,true);
    view->setColumnHidden(1,true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setShowGrid(false);
    view->setGridStyle(Qt::NoPen);
    view->setSortingEnabled(false);
    view->verticalHeader()->setVisible(false);
    view->horizontalHeader()->setCascadingSectionResizes(true);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void PlayListEditor::on_searchButton_clicked()
{
    emit searchMusic(ui->searchLine->text().split(" ", QString::SkipEmptyParts));
}

void PlayListEditor::on_searchLine_returnPressed()
{
    on_searchButton_clicked();
}

void PlayListEditor::setName(QString name){
    ui->name->setText(name);
}

void PlayListEditor::configure(){
    configureSelectedMusics();
    emit searchMusic(QStringList());

    QSqlQuery query(QSqlDatabase::database());
    QDateTime timestamp;

    // Get Timestamp
    query.prepare("SELECT current_timestamp");
    query.exec();
    query.first();
    timestamp = query.value(0).toDateTime();
    query.finish();
    query.clear();

    query.prepare("INSERT INTO playlist(nome, data, private, descricao, utilizador_nick) "
                  "VALUES(?, ?, ?, ?, ?)");
    query.addBindValue(ui->name->text());
    query.addBindValue(timestamp);
    query.addBindValue(ui->setPrivate->isChecked());
    query.addBindValue(ui->description->toPlainText());
    query.addBindValue(currentUser);
    if(!query.exec()){
        QSqlDatabase::database().rollback();
        emit failure();
        this->reject();
        return;
    }
}


void PlayListEditor::on_removeMusicButton_clicked()
{
    QSqlRelationalTableModel *playlistModel = qobject_cast<QSqlRelationalTableModel*>(ui->selectedMusics->model());

    if(ui->allMusicsTableView->currentIndex().isValid()){
        //VERIFICAR SE JÁ EXISTE
        QModelIndex removeIndex = ui->selectedMusics->currentIndex();
        int row = removeIndex.row() == -1 ? 0 : removeIndex.row();
        playlistModel->removeRow(row);
    }
}

void PlayListEditor::on_addMusicButton_clicked()
{
    QSqlQueryModel *musicModel = qobject_cast<QSqlQueryModel*>(ui->allMusicsTableView->model());
    QSqlRelationalTableModel *playlistModel = qobject_cast<QSqlRelationalTableModel*>(ui->selectedMusics->model());

    if(ui->allMusicsTableView->currentIndex().isValid()){
        //VERIFICAR SE JÁ EXISTE
        QModelIndex insertIndex = ui->selectedMusics->currentIndex();
        int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
        playlistModel->insertRow(row);
        insertIndex = playlistModel->index(row, 0);

        ui->selectedMusics->setColumnHidden(0,false);
        ui->selectedMusics->setColumnHidden(1,false);
        playlistModel->setData(playlistModel->index(row, 0),ui->name->text());
        playlistModel->setData(playlistModel->index(row, 1),currentUser);

        ui->allMusicsTableView->setColumnHidden(0,false);
        playlistModel->setData(playlistModel->index(row, 2), musicModel->itemData(ui->allMusicsTableView->currentIndex().siblingAtColumn(0)).value(0,-1).toInt());
        ui->selectedMusics->setColumnHidden(0,true);
        ui->selectedMusics->setColumnHidden(1,true);
        ui->allMusicsTableView->setColumnHidden(0,true);
        ui->selectedMusics->setCurrentIndex(insertIndex);
    }

}
