#include "browsemusicdialog.h"
#include "ui_browsemusicdialog.h"

BrowseMusicDialog::BrowseMusicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowseMusicDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen);
}

BrowseMusicDialog::~BrowseMusicDialog()
{
    delete ui;
}
