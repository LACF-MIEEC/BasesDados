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

signals:
    void acceptedUser(QString username);

private slots:
    void on_LogInButton_clicked();

    void on_RegisterButton_clicked();

    void on_DBSettings_clicked();

    QString userName() const;

    QString passWord() const;

    bool saveCredentialsSet() const;

private:
    Ui::StartWindow *ui;
};

#endif // STARTWINDOW_H
