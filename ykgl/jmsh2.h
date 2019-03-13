#ifndef JMSH2_H
#define JMSH2_H

#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class Jmsh2;
}

class Jmsh2 : public CommonWidget
{
    Q_OBJECT

public:
    explicit Jmsh2(QWidget *parent = 0);
    ~Jmsh2();
    void init();
private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_pre_clicked();

    void on_pushButton_next_clicked();

private:
    Ui::Jmsh2 *ui;
};

#endif // JMSH2_H
