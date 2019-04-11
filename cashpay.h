#ifndef CASHPAY_H
#define CASHPAY_H
#include"operatefile.h"
#include <QWidget>
#include "common/commonwidget.h"
#include"homepage.h"
#include "dealfinish.h"
namespace Ui {
class CashPay;
}

class CashPay : public CommonWidget
{
    Q_OBJECT

public:
    explicit CashPay(QWidget *parent = 0);
    ~CashPay();
    void init();
    void startTimer();
signals:
   void back_MsgConfirm();
   void back_HomePage();
   void start(long msec);
   void stop();
   void updateDispaly();
private:
    Ui::CashPay *ui;
    QTimer *timer;
    QTimer *timer2;
    unsigned int currentsum;
    unsigned int totalsum;
    QString dllsh;
    dealfinish *form_dealfinish;
    QString yl_result;
    QStringList m_HttpData;
    QString m_errorstr;
    int  m_currenttime;
    int bill_error_times;
    bool IsAccaptResponse;
    enum BILLSTATE_t
    {
        BEGIN,
        END
    };
    BILLSTATE_t BILLSTATE;
    enum BILLCHECKSTATE_t
    {
        NOMCHECK,
        LASCHECK,
        SECCHECK,
    };
    BILLCHECKSTATE_t BILLCHECKSTATE;
public slots:
    void cashPay_GetData_from_EC();
    void cashPay_DealData(KIND reply);
    void cashPay_GetData_form_bill();
    void check_CurrenMoney();
    void check_TotalMoney();
    void readywriteCard();
    void connectError(QAbstractSocket::SocketError socketError);
private slots:
    void on_pushButton_clicked();
    void setLcdnum();
    void waitTimeout();
    void on_pushButton_beginpay_clicked();
    void replyFinished(QNetworkReply* reply);
    void replyFinished_writecard(QNetworkReply *reply);
    void GetSerialNumber();
    void disconnectSlots();
    void backHomeBroken();
    void cancelPay(const QString &Errorstr);
    void writecard();


    void on_pushButton_purchase_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();

protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // CASHPAY_H
