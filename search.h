#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

private slots:
    void on_SearchB_clicked();

    void on_table_doubleClicked(const QModelIndex &index);

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
