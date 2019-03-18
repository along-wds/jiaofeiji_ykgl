#ifndef HOMEPAGE_H
#define HOMEPAGE_H
#include"operatefile.h"
#include"clientsocket.h"
#include"common/commondialog.h"
#include"chiocedialog.h"
#include <QWidget>
#include"purchase.h"
#include "src/timertask.h"
#include"manager.h"
#include "readcard.h"
#include "webpage.h"
//#define DEBUG
extern commonDialog *messagebox;
extern ClientSocket *socket;
extern Timertask *timertask;
extern Manager *ui_manager;
namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = 0);
    ~HomePage();
     void init(int flag=0);
     void dealDeviceBroken(DeviceError ErrorType);
     void ejectCard();
     QString warnMsg;
     bool Couldpurchase;
signals:
    void back_first();
    void start(long msec);
    void stop();
    void updateDispaly();
    void setTaskstate(bool);
    void startReadCardTimer();
    void clean_msg();
    void reCheck();
private slots:
    void on_pushButton_purchase_clicked();
    void on_pushButton_search_clicked();

    void on_pushButton_writecard_clicked();

    void on_pushButton_zdbd_clicked();

protected:
   void mousePressEvent(QMouseEvent *event);
private:
    Ui::HomePage *ui;
    PurChase *form_PurChase;
    int m_currenttime;
    bool step1;
    bool step2;
    bool step3;
    bool isjump;
    QTimer *timer;
    void startTimer();
    void disconnectSlots();
public slots:
    void readyReadCard();
    void getCardMsg(QString s_msg);

};

#endif // HOMEPAGE_H
