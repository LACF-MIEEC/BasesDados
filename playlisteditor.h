#ifndef PLAYLISTEDITOR_H
#define PLAYLISTEDITOR_H

#include <QDialog>

namespace Ui {
class PlayListEditor;
}

class PlayListEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PlayListEditor(QWidget *parent = nullptr);
    ~PlayListEditor();

private:
    Ui::PlayListEditor *ui;
};

#endif // PLAYLISTEDITOR_H
