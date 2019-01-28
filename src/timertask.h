#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QObject>
#include"QtNetwork"
#include"qtimer.h"
#include"clientsocket.h"
class Timertask:public ClientSocket
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
