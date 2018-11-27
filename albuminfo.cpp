#include "albuminfo.h"
#include "ui_albuminfo.h"

AlbumInfo::AlbumInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlbumInfo)
{
    ui->setupUi(this);
}

AlbumInfo::~AlbumInfo()
{
    delete ui;
}
