#ifndef HEARTBEATTASK_H
#define HEARTBEATTASK_H

#include <QObject>
#include"usermsg.h"
#include<QtNetwork>
//#include"QTimer"
class HeartbeatTask:public QObject
{
    Q_OBJECT
public:

    explicit HeartbeatTask();
    void sendMessage();
    void HttpSend(const QUrl &url);
    Login LoginMessage;
signals:

private slots:
    //void stopSendHeartBeat();
    void sendHeartbeatPack();
    void recvMessage();
    void init();
    void connectError(QAbstractSocket::SocketError socketError);
    void openTimer();
    void connectToServer();
    void launchHeartBeatServer();
    void send2Yyt();
    void replyFinished(QNetworkReply *reply);
private:
    QDateTime recvtime;
    QTimer *check_server_timer; //检测守护程序
    QTimer *conn_launch_timer; //连接到守护
    QTimer *conn_yyt_timer;    //连接到营业综合服务平台
    QString ip;
    QString port;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QTcpSocket HeartSocet;
};

#endif // HEARTBEATTASK_H
