#ifndef HEATERLOOKUP_H
#define HEATERLOOKUP_H

#include <QWidget>
#include "common/commonwidget.h"

namespace Ui {
class Heaterlookup;
}

class Heaterlookup : public CommonWidget
{
    Q_OBJECT

public:
    explicit Heaterlookup(QWidget *parent = 0);
    ~Heaterlookup();
     void init();
     void startTimer();
signals:
    void start(long msec);
    void stop();
     void updateDispaly();
private:
    Ui::Heaterlookup *ui;
     int  m_currenttime;
protected:
 //  void mousePressEvent(QMouseEvent *event);
private slots:
   // void setLcdnum();
    //void waitTimeout();
};

#endif // HEATERLOOKUP_H
