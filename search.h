#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QString>
#include <QObject>

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();
    QString val[3];

signals:
    void sendParams(QString s[3]);

private slots:
    void on_SearchB_clicked();

    void on_table_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::Search *ui;    
};

#endif // SEARCH_H
