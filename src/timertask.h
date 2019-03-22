#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QObject>
#include"QtNetwork"
#include"qtimer.h"
#include"clientsocket.h"
class Timertask:public CommonSocket
{
    Q_OBJECT
public:
    Timertask();
    int checkCard(QString &ickarr);
    int checkCard(QString&, QString&, QString &, QString &);
    ~Timertask();
public slots:
    void receiveHttpMsg(QNetworkReply* reply);
    void sendHeartbeatPack();
    void init();
    void checkElectricCard();
    void Timertask_GetData();
    void Timertask_cleanMsg();
    void Timertask_reCheckCard();
    void dealReadCardTimeout();
    void StartTimer();
    void connectError(QAbstractSocket::SocketError socketError);
    void setCardState(const bool &state);
private:

    QTimer *timer;
    QTimer *Check_Card_timer;
    QTimer *Check_Readcard_timer;
    QStringList m_HttpData;
    bool ishascard;
    bool isSendReadCard;
    QString CurrentCardString;
public:
    Fkcard FK_msg;          //读卡后详细字段数据
    Lkcard LK_msg;
    Cardbase Card_Basemsg;
    //int PurchaseType;       //缴费类型
    bool Unionpaychannel;
    int CardType;           //卡类型
    YLmsg YL_msg;
    Login LoginMessage;
    Usermessage message;
    QString ReadCard_str;
signals:
    void  hascard();
    void  hascardmsg(QString);
public slots:
    void setTimerstate(bool state);
private slots:
    void replyFinished_timer(QNetworkReply *reply);
    void replyFinished_detailMsg(QNetworkReply *reply);
};
#endif // TIMERTASK_H
