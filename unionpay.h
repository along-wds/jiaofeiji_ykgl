#ifndef UNIONPAY_H
#define UNIONPAY_H
#include <QWidget>
#include<QTimer>
#include"operatefile.h"
#include"clientsocket.h"
#include"inputpassword.h"
#include"homepage.h"
#include"usermsg.h"
extern ClientSocket *socket;
namespace Ui {
class UnionPay;
}

class UnionPay : public QWidget
{
    Q_OBJECT
public:
    explicit UnionPay(QWidget *parent = 0);
    ~UnionPay();
    bool loadlib();
    void registtoUnion();
    void init();
private:
    Ui::UnionPay *ui;
    int m_currenttime;
signals:
    void back_MsgConfirm();
    void back_heatermsgconfirm();
    void start(long msec);
    void stop();
    void updateDispaly();
    void initwithUnion();
public slots:
    void setLcdnum();
   // void unionPay_GetData();
    //void unionPay_DealData();
    void unionPay_reshow();
    void startTimer();
    void getResp(int ret);
private slots:
    void on_pushButton_back_clicked();
    void onTimerout();
    void waitTimeout();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();

private:
    void OpenDev();
    void ReadCard();
    void CloseDev();
    void getCardNum();
    void disconnectSlots();
    void HandleError(int m_errorcode);
    void loadTolib();
    char *cardid;
    inputpassword *form_inputpassword;
    paystr_in InData;
    QTimer *timer;
protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // UNIONPAY_H
