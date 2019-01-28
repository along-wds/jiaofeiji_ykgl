#ifndef MSGCONFIRM_H
#define MSGCONFIRM_H
#include <QWidget>
#include"clientsocket.h"
#include<QtNetwork>
#include"usermsg.h"
#include"homepage.h"
extern HomePage *homepage;
extern ClientSocket *socket;
extern ChioceDialog *choicebox;
namespace Ui {
class MsgConfirm;
}

class MsgConfirm : public QWidget
{
    Q_OBJECT

public:
    explicit MsgConfirm(QWidget *parent = 0);
    ~MsgConfirm();
   //  ClientSocket *socket;
    void display();
    void init();
    void hideTzje();
    void showTzje();
signals:
    void back_readcard();
    void start(long msec);
    void stop();
    void updateDispaly();
private slots:
    void setLcdnum();
    void waitTimeout();
    void msgconfirm_reshow();
    void msgconfirm_GetData();
    void replyFinished(QNetworkReply* reply);
    void on_pushButton_ConfirmPurchase_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_backHomePage_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_ylPurchase_clicked();
    void writecard();
    //void msgconfirm_GetData();
private:
    Usermessage displaymsg;
     int purchasetype;
    Ui::MsgConfirm *ui;
    QStringList httpData;
    int m_currenttime;
    int times;//判断进入次数
    QStringList m_HttpData;
protected:
   void mousePressEvent(QMouseEvent *event);
private:
  void startTimer();
  void disconnectSlots();
public slots:
  void jumpToWriteList();
  void closeChoiceDialog();

};

#endif // MSGCONFIRM_H
