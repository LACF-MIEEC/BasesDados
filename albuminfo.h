#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QDialog>

namespace Ui {
class AlbumInfo;
}

class AlbumInfo : public QDialog
{
    Q_OBJECT

public:
    explicit AlbumInfo(QWidget *parent = nullptr);
    ~AlbumInfo();

private:
    Ui::AlbumInfo *ui;
};

#endif // ALBUMINFO_H
