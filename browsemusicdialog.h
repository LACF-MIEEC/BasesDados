#ifndef BROWSEMUSICDIALOG_H
#define BROWSEMUSICDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class BrowseMusicDialog;
}

class BrowseMusicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseMusicDialog(QWidget *parent = nullptr);
    ~BrowseMusicDialog();

    int getSelectedID(){return selectedID;}
    QString getSelectedName(){return  selectedName;}
    QString getSelectedInterpreter(){return selectedInterpreter;}

    void setMusics(QSqlQuery * query);
    void showAllMusics();

signals:
    void searchMusic(QStringList);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_searchButtonBrowseMusic_clicked();

    void on_searchLineBrowseMusic_returnPressed();


private:
    Ui::BrowseMusicDialog *ui;

    int selectedID;
    QString selectedName;
    QString selectedInterpreter;


};

#endif // BROWSEMUSICDIALOG_H
