#ifndef FIRST_H
#define FIRST_H
#include <QWidget>
#include<QTimer>
#include"homepage.h"
#include"heater/heaterhomepage.h"
namespace Ui {
class First;
}

class First : public CommonWidget
{
    Q_OBJECT
public:
    explicit First(QWidget *parent = 0);
    ~First();
    void checkCard();
    void dealDeviceBroken(DeviceError ErrorType);
    void ejectCard();
    void init();
    bool Couldpurchase;
private slots:
    void on_pushButton_2_clicked();
    void reshow_first();
    void on_pushButton_clicked();
    void Checktimer_out();
    //bool eventFilter(QObject *,QEvent *);    //注意这里

private:
    Ui::First *ui;
    HomePage *form_homepage;
    Heaterhomepage *form_heaterhomepage;
    QRect rect;
    QTimer *CheckTimer;
    int CheckTimes;
    bool step1;
    bool step2;
    bool step3;

protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // FIRST_H
