#include "myprofile.h"
#include "ui_myprofile.h"

MyProfile::MyProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyProfile)
{
    ui->setupUi(this);
}

MyProfile::~MyProfile()
{
    delete ui;
}
