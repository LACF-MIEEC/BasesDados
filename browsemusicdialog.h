#ifndef BROWSEMUSICDIALOG_H
#define BROWSEMUSICDIALOG_H

#include <QDialog>

namespace Ui {
class BrowseMusicDialog;
}

class BrowseMusicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseMusicDialog(QWidget *parent = nullptr);
    ~BrowseMusicDialog();

private:
    Ui::BrowseMusicDialog *ui;
};

#endif // BROWSEMUSICDIALOG_H
