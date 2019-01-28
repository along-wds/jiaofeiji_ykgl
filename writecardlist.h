#ifndef WRITECARDLIST_H
#define WRITECARDLIST_H
#include"clientsocket.h"
#include"homepage.h"
#include"usermsg.h"
#include <QWidget>
#include<QTableWidget>
#include<QtNetwork>
#include"qbuttongroup.h"
extern ClientSocket *socket;
extern HomePage *homepage;
namespace Ui {
class Writecardlist;
}
class Writecardlist : public QWidget
{
    Q_OBJECT
public:
    explicit Writecardlist(QWidget *parent = 0);
    ~Writecardlist();
    void setList();
    void setRows();
signals:
    void homepage_back();
     void start(long msec);
     void restart(long msec);
     void stop();
     void updateDispaly();
private:
    Ui::Writecardlist *ui;
    QTableWidget *table_widget;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QStringList m_HttpData;
    int rows;
    QString selecttext;
    FKwithcard FK_writewithcard;
    LKwithcard LK_writewithcard;
    //LKwithcard LK_writewithcard;
    FKdatacheck FK_datacheck;
    LKdatacheck LK_datacheck;
    int writetimes;
    bool write_button;
    int m_currenttime;
    QButtonGroup *btnGroup;
public:
    Cardbase unwrite_cardmsg;
    FKwithcard unwrite_msg;
private slots:
    void writecardlist_GetData();
    void replyFinished(QNetworkReply* reply);
    void replyFinished_writecard(QNetworkReply* reply);
    void replyFinished_carddatasearch(QNetworkReply* reply);
    //void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void waitTimeout();
    void setLcdnum();
    void on_pushButton_3_clicked();
    void MouseTrackItem(int);
    void changeCheckstate(int,int,int,int);

    void on_pushButton_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();

protected:
   void mousePressEvent(QMouseEvent *event);
private:
   void startTimer();
   void disconnectSlots();
};

#endif // WRITECARDLIST_H
