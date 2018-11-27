#include "playlisteditor.h"
#include "ui_playlisteditor.h"

PlayListEditor::PlayListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayListEditor)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen);
}

PlayListEditor::~PlayListEditor()
{
    delete ui;
}
