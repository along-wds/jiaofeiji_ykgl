#ifndef HEATERDEALFINISHED_H
#define HEATERDEALFINISHED_H

#include <QWidget>
#include "common/commonwidget.h"

namespace Ui {
class Heaterdealfinished;
}

class Heaterdealfinished : public CommonWidget
{
    Q_OBJECT

public:
    explicit Heaterdealfinished(QStringList &m_httpData,QWidget *parent = 0);
    ~Heaterdealfinished();
    void display();
     void init();
     void startTimer();
protected:
   void mousePressEvent(QMouseEvent *event);
private slots:
    //void on_pushButton_ConfirmPurchase_clicked();
    void setLcdnum();
    void waitTimeout();
    void on_pushButton_ConfirmPurchase_4_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_Print_clicked();
    void HeaterDealfinished_GetData();
signals:
    void start(long msec);
    void stop();
    void updateDispaly();
private:
    Ui::Heaterdealfinished *ui;
    QStringList httpData;
    int  m_currenttime;

};

#endif // HEATERDEALFINISHED_H
