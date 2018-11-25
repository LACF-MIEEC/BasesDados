#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString currentUser;


private slots:
    void loggedIn(QString);
    void closeEvent(QCloseEvent *event);
    void refreshAll();
    void refreshUserPanel();


    void on_homeButton_clicked();
    void on_filesButton_clicked();
    void on_searchButton_clicked();
    void on_searchBar_returnPressed();
    void on_playlistsButton_clicked();
    void on_newPrivatePlayList_clicked();
    void on_chooseMusicButton_clicked();
};

#endif // MAINWINDOW_H
