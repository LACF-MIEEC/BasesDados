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

    void setAllMusics(QSqlQuery * query);
    void setName(QString name);
    void setUsername(QString username){currentUser=username;}
    void configure();

signals:
    void searchMusic(QStringList);
    void failure();

private slots:
    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

    void configureSelectedMusics();

    void on_searchButton_clicked();

    void on_searchLine_returnPressed();

    void on_removeMusicButton_clicked();

    void on_addMusicButton_clicked();

private:
    Ui::PlayListEditor *ui;
    QString currentUser;
};

#endif // PLAYLISTEDITOR_H
