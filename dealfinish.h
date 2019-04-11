#ifndef DEALFINISH_H
#define DEALFINISH_H
#include<QtNetwork>
#include <QWidget>
#include "common/commonwidget.h"
#include"clientsocket.h"
#include"homepage.h"
extern ClientSocket *socket;
namespace Ui {
class dealfinish;
}
class dealfinish : public CommonWidget
{
    Q_OBJECT

public:
    explicit dealfinish(QWidget *parent = 0);
    ~dealfinish();
    void startTimer();
signals:
   void back_HomePage();
   void start(long msec);
   void stop();
   void updateDispaly();
private:
    Ui::dealfinish *ui;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QStringList m_HttpData;
    int  m_currenttime;
    QHash<int,QString>header;
    QString PrintString;
private slots:
    void replyFinished(QNetworkReply* reply);
    void InvoicePrinter_replyFinished(QNetworkReply *reply);
    void dealfinish_GetData();
    void get_InvoicePrinter_Data();
    //void on_pushButton_ConfirmPurchase_3_clicked();
    void waitTimeout();
    void on_pushButton_2_clicked();
    void setLcdnum();
    void on_pushButton_backHomePage_2_clicked();

    void on_pushButton_back_2_clicked();
    void  disconnectSlots();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();

public:
     void display();
     void init(int initType);
     void init();
     void display_with_WriteCard();
     void display_without_writecard();
protected:
   void mousePressEvent(QMouseEvent *event);
private:

};

#endif // DEALFINISH_H
