#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QDialog>

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private slots:
    void on_LogInButton_clicked();

    void on_RegisterButton_clicked();

    void on_DBSettings_clicked();

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
