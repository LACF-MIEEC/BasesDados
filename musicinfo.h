#ifndef MUSICINFO_H
#define MUSICINFO_H

#include <QDialog>
#include "search.h"
namespace Ui {
class MusicInfo;
}

class MusicInfo : public QDialog
{
    Q_OBJECT

public:
    explicit MusicInfo(QWidget *parent = nullptr);
    ~MusicInfo();

private slots:
    void on_MainB_clicked();

    void on_GoBackB_clicked();

    void on_ArtistB_clicked();

    void on_MainB_2_clicked();

signals:
    void send_values(QString s[3]);

public slots:
    void receive(QString s[3]);


private:
    Ui::MusicInfo *ui;
    QString valm[3];
    QString nick;
};

#endif // MUSICINFO_H
