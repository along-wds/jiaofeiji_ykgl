#ifndef JMSH3_H
#define JMSH3_H

#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class Jmsh3;
}

class Jmsh3 : public CommonWidget
{
    Q_OBJECT

public:
    explicit Jmsh3(QWidget *parent = 0);
    ~Jmsh3();
    void init();
private slots:
    void on_pushButton_pre_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_end_clicked();

    void on_pushButton_back_clicked();

private:
    Ui::Jmsh3 *ui;
};

#endif // JMSH3_H
