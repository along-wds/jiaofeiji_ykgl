#ifndef HEATERHOMEPAGE_H
#define HEATERHOMEPAGE_H

#include <QWidget>
#include "common/commonwidget.h"
#include"readnum.h"
#include"clientsocket.h"
#include"manager.h"
extern ClientSocket *socket;
extern Manager *ui_manager;
namespace Ui {
class Heaterhomepage;
}

class Heaterhomepage : public CommonWidget
{
    Q_OBJECT
public:
    explicit Heaterhomepage(QWidget *parent = 0);
    ~Heaterhomepage();
     void init();
     void startTimer();
signals:
     void back_first();
     void start(long msec);
     void stop();
     void updateDispaly();
private slots:
    void on_pushButton_backfirst_clicked();
    void on_pushButton_search_clicked();
    void reshow_heaterhomepage();
    void on_pushButton_purchase_clicked();
    void setLcdnum();
    void waitTimeout();
protected:
   void mousePressEvent(QMouseEvent *event);
private:
    Ui::Heaterhomepage *ui;
    Readnum *form_readnum;
    int  m_currenttime;

};

#endif // HEATERHOMEPAGE_H
