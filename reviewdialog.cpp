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

    score = 1;
}

void ReviewDialog::on_Star2_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(false);
    ui->Star4->setChecked(false);
    ui->Star5->setChecked(false);

    score = 2;
}

void ReviewDialog::on_Star3_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(false);
    ui->Star5->setChecked(false);

    score = 3;
}

void ReviewDialog::on_Star4_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(true);
    ui->Star5->setChecked(false);

    score = 4;
}

void ReviewDialog::on_Star5_clicked()
{
    ui->Star1->setChecked(true);
    ui->Star2->setChecked(true);
    ui->Star3->setChecked(true);
    ui->Star4->setChecked(true);
    ui->Star5->setChecked(true);

    score = 5;
}

void ReviewDialog::on_buttonBox_accepted()
{
    this->accept();
}

void ReviewDialog::on_buttonBox_rejected()
{
    this->reject();
}

int ReviewDialog::getScore(){
    return score;
}

QString ReviewDialog::getReview(){
    return ui->reviewComment->toPlainText();
}

void ReviewDialog::setMusicAuthor(QString music, QString author){
    ui->MusicAuthor->setText(music + " - " + author);
}
