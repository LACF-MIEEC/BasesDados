#include "browsemusicdialog.h"
#include "ui_browsemusicdialog.h"

#include <QSqlQueryModel>

BrowseMusicDialog::BrowseMusicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowseMusicDialog)
{
    ui->setupUi(this);

    QTableView *view = ui->browseMusicTableView;
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

BrowseMusicDialog::~BrowseMusicDialog()
{
    delete ui;
}

void BrowseMusicDialog::on_buttonBox_accepted()
{
    if(ui->browseMusicTableView->currentIndex().isValid()){
        ui->browseMusicTableView->setColumnHidden(0,false);
        selectedID = ui->browseMusicTableView->model()->itemData(ui->browseMusicTableView->currentIndex().siblingAtColumn(0)).value(0,-1).toInt();
        selectedName = ui->browseMusicTableView->model()->itemData(ui->browseMusicTableView->currentIndex().siblingAtColumn(1)).value(0,-1).toString();
        selectedInterpreter = ui->browseMusicTableView->model()->itemData(ui->browseMusicTableView->currentIndex().siblingAtColumn(3)).value(0,-1).toString();
        ui->browseMusicTableView->setColumnHidden(0,true);

        this->accepted();
    }
}

void BrowseMusicDialog::on_buttonBox_rejected()
{
    this->reject();
}

void BrowseMusicDialog::on_searchButtonBrowseMusic_clicked()
{
    emit searchMusic(ui->searchLine->text().split(" ", QString::SkipEmptyParts));
}

void BrowseMusicDialog::on_searchLineBrowseMusic_returnPressed()
{
    on_searchButtonBrowseMusic_clicked();
}

void BrowseMusicDialog::setMusics(QSqlQuery * query){

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel *>(ui->browseMusicTableView->model());
    model->setQuery(*query);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Album"));
    model->setHeaderData(3, Qt::Horizontal, tr("Interpreter"));
    ui->browseMusicTableView->setColumnHidden(0, true);
}

void BrowseMusicDialog::showAllMusics(){
    emit searchMusic(QStringList());
}
