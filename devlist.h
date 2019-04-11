#ifndef DEVLIST_H
#define DEVLIST_H

#include <QWidget>
#include "common/commonwidget.h"
#include"qsqltablemodel.h"
namespace Ui {
class Devlist;
}

class Devlist : public CommonWidget
{
    Q_OBJECT

public:
    explicit Devlist(QWidget *parent = 0);
    ~Devlist();
    void setList();
    void getSum();
    void init();
private slots:

    void on_pushButton_clicked();

    void on_pushButton_search_clicked();

private:
    Ui::Devlist *ui;
    int row;
    QSqlTableModel model;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DEVLIST_H
