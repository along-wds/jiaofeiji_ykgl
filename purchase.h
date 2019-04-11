#ifndef PURCHASE_H
#define PURCHASE_H
#include"clientsocket.h"
#include <QWidget>
#include "common/commonwidget.h"
//extern ClientSocket *socket;
namespace Ui {
class PurChase;
}
class PurChase : public CommonWidget
{
    Q_OBJECT

public:
    explicit PurChase(QWidget *parent = 0);
    ~PurChase();
    void startTimer();
    void init();
    void disconnectSlots();
signals:
    void back_HomePage();
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();
private slots:
    void on_pushButton_cashPay_clicked();
    //void on_pushButton_backHomePage_clicked();
    void Purchase_reshow();
    void on_pushButton_unionPay_clicked();

    void on_pushButton_writeCard_clicked();

    void on_pushButton_backHomePage_2_clicked();

    void on_pushButton_backHomePage_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void setLcdnum();

    void waitTimeout();


private:
    Ui::PurChase *ui;
    int PurChaseType;
    int m_currenttime;
protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // PURCHASE_H
