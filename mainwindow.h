#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QSqlRelationalTableModel>

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
    QVariant currentPK;
    int currentFileMusicID;

    QList<QWidget*> stackedWidgetHistory;
    int stackedWidgetHistoryIndex;


private slots:
    void loggedIn(QString);
    void closeEvent(QCloseEvent *event);
    void refreshUserPanel();
    void refreshNotifications();
    void refreshFiles();
    void refreshPlaylists();

    void databaseFailure();
    void playlistCreated();
    void cancelPlaylist();
    void refreshPlaylistEditorBrowser(QStringList keywords);

    void refreshMusicBrowser(QStringList keywords);
    void cancelMusicSelection();
    void musicSelected();

    int searchMusic(QStringList keywords);
    int searchAlbum(QStringList keywords);
    int searchPlaylist(QStringList keywords);
    int searchConcert(QStringList keywords);
    int searchInterpreter(QStringList keywords);
    QSqlQuery* buildSearchQuery(QString searchQuery, QStringList keywords);


    void configureTableView(QTableView* view);
    void closeAllViews();
    void configureAllSqlTableView();



    void on_homeButton_clicked();
    void on_filesButton_clicked();
    void on_searchButton_clicked();
    void on_searchBar_returnPressed();
    void on_playlistsButton_clicked();
    void on_newPrivatePlayList_clicked();
    void on_chooseMusicButton_clicked();
    void on_reviewAlbumButton_clicked();
    void on_returnButton_clicked();
    void setCurrentPage(QWidget* page);
    void on_fowardButton_clicked();
    void on_musicTableView_clicked(const QModelIndex &index);
    void on_reviewMusicButton_clicked();
    void on_newPublicPlayList_clicked();
    void on_editPrivatePlaylist_clicked();
    void on_deletePrivatePlayList_clicked();
    void on_deletePublicPlayList_clicked();
    void on_browseButton_clicked();
    void on_addFileButton_clicked();
    void on_albumsTableView_clicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
