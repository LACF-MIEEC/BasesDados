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

void BrowseMusicDialog::on_buttonBox_accepted()
{
    this->accepted();
}

void BrowseMusicDialog::on_buttonBox_rejected()
{
    this->reject();
}
