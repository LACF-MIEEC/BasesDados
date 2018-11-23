#ifndef MYPROFILE_H
#define MYPROFILE_H

#include <QDialog>

namespace Ui {
class MyProfile;
}

class MyProfile : public QDialog
{
    Q_OBJECT

public:
    explicit MyProfile(QWidget *parent = nullptr);
    ~MyProfile();

private:
    Ui::MyProfile *ui;
};

#endif // MYPROFILE_H
