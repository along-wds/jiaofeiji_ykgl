#ifndef DEVLIST_H
#define DEVLIST_H

#include <QWidget>

namespace Ui {
class Devlist;
}

class Devlist : public QWidget
{
    Q_OBJECT

public:
    explicit Devlist(QWidget *parent = 0);
    ~Devlist();
    void setList();
private:
    Ui::Devlist *ui;
    int row;
};

#endif // DEVLIST_H
