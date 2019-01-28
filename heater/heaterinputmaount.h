#ifndef HEATERINPUTMAOUNT_H
#define HEATERINPUTMAOUNT_H
#include <QWidget>
#include<QtNetwork>
#include"heater/heaterdealfinished.h"
namespace Ui {
class Heaterinputmaount;
}

class Heaterinputmaount : public QWidget
{
    Q_OBJECT

public:
    explicit Heaterinputmaount(QWidget *parent = 0);
    ~Heaterinputmaount();
     void startTimer();
     double round(double d, unsigned p);
     void  disconnectSlots();
     void init();
     void backHomeBroken();
private slots:
    void on_pushButton_back_homepage_clicked();
    void replyFinished(QNetworkReply*);
    void replyFinished_unifyplatform(QNetworkReply *);
    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_BackSapce_3_clicked();

    void on_pushButton_Reset_3_clicked();

    void on_pushButton_Ok_3_clicked();
    void setLcdnum();
    void waitTimeout();
    void on_pushButton_4_clicked();
    void on_pushButton_00_clicked();
    void on_pushButton_3_clicked();

    void  GetSerialNumber();
    void  cancelPay(const QString &Errorstr);
    void  getData();
    void getRespcode(QString amount);
private:
    Ui::Heaterinputmaount *ui;
     int  m_currenttime;
     QString dllsh;
     int m_recvtimes;
     Heaterdealfinished *form_heaterdealfinished;
     QString m_je;
     QString m_errorstr;
     QString yl_result;
private:
     void checkAmount();
     void finalCheck();
     int bitCheck();
     char* mid(int m, int n);
protected:
   void mousePressEvent(QMouseEvent *event);
signals:
    void start(long msec);
    void stop();
    void updateDispaly();
    void heaterDealwithUnion(QString amount);
public:
     QStringList m_HttpData;

};

#endif // HEATERINPUTMAOUNT_H
