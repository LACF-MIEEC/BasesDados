#include "playlisteditor.h"
#include "ui_playlisteditor.h"

#include <QSqlQueryModel>

PlayListEditor::PlayListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayListEditor)
{
    ui->setupUi(this);

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
}

PlayListEditor::~PlayListEditor()
{
    delete ui;
}

void PlayListEditor::on_buttonBox_2_accepted()
{
    //VERIFICAR INTEGRIDADE DOS DADOS
    emit acceptedPlayList();
    this->accept();
}
s
void PlayListEditor::on_buttonBox_2_rejected()
{
    this->reject();
}

QString PlayListEditor::getPlaylistName(){
    return ui->name->text();
}

QString PlayListEditor::getPlaylistDescription(){
    return ui->description->toPlainText();
}

QVariantList PlayListEditor::getPlaylistMusics(){
    return selectedMusicsID;
}

bool PlayListEditor::getPlaylistPrivate(){
    return ui->setPrivate->isChecked();
}

void PlayListEditor::setAllMusics(QSqlQuery * query){
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->allMusicsTableView->model());

}
