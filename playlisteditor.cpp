#include "playlisteditor.h"
#include "ui_playlisteditor.h"

PlayListEditor::PlayListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayListEditor)
{
    ui->setupUi(this);
}

PlayListEditor::~PlayListEditor()
{
    delete ui;
}
