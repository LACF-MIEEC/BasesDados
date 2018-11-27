#include "reviewdialog.h"
#include "ui_reviewdialog.h"

ReviewDialog::ReviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReviewDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen);
}


ReviewDialog::~ReviewDialog()
{
    delete ui;
}



void ReviewDialog::on_Star1_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(false);
    ui->Star3->setChecked(false);
    ui->Star4->setChecked(false);
    ui->Star5->setChecked(false);

    rating = 1;
}

void ReviewDialog::on_Star2_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(false);
    ui->Star4->setChecked(false);
    ui->Star5->setChecked(false);

    rating = 2;
}

void ReviewDialog::on_Star3_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(false);
    ui->Star5->setChecked(false);

    rating = 3;
}

void ReviewDialog::on_Star4_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(true);
    ui->Star5->setChecked(false);

    rating = 4;
}

void ReviewDialog::on_Star5_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(true);
    ui->Star5->setChecked(true);

    rating = 5;
}
