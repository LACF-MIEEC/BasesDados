#ifndef REVIEWDIALOG_H
#define REVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewDialog(QWidget *parent = nullptr);
    ~ReviewDialog();

private slots:
    void on_Star1_clicked();
    void on_Star2_clicked();
    void on_Star3_clicked();
    void on_Star4_clicked();
    void on_Star5_clicked();

private:
    Ui::ReviewDialog *ui;
    int rating;
};

#endif // REVIEWDIALOG_H
