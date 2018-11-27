#ifndef MUSICREVIEW_H
#define MUSICREVIEW_H

#include <QDialog>

namespace Ui {
class MusicReview;
}

class MusicReview : public QDialog
{
    Q_OBJECT

public:
    explicit MusicReview(QWidget *parent = nullptr);
    ~MusicReview();

public slots:
    void receive(QString s[2]);
private slots:
    void on_ArtistB_4_clicked();

    void on_GoBackB_4_clicked();

private:
    Ui::MusicReview *ui;
    QString param[2];
};

#endif // MUSICREVIEW_H
