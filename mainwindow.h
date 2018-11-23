#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_MyProfile_clicked();
    void on_MyProfile_hover();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
