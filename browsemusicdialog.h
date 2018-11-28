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

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BrowseMusicDialog *ui;
};

#endif // BROWSEMUSICDIALOG_H
