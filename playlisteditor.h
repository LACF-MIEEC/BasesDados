#ifndef PLAYLISTEDITOR_H
#define PLAYLISTEDITOR_H

#include <QDialog>
#include <QSqlQuery>
namespace Ui {
class PlayListEditor;
}

class PlayListEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PlayListEditor(QWidget *parent = nullptr);
    ~PlayListEditor();

    QString getPlaylistName();
    QString getPlaylistDescription();
    QVariantList getPlaylistMusics();
    bool getPlaylistPrivate();

    void setAllMusics(QSqlQuery * query);
    void setDatabaseMusics(QVariantList musicList);

signals:
    void acceptedPlayList();

private slots:
    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

private:
    Ui::PlayListEditor *ui;
    QList<int> selectedMusicsID;
};

#endif // PLAYLISTEDITOR_H
