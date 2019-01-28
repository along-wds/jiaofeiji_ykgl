#ifndef HEARTBEATTASK_H
#define HEARTBEATTASK_H

#include <QObject>
#include"homepage.h"
class HeartbeatTask : public QTcpSocket
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
private:
    QDateTime recvtime;
    QTimer *timer_check_server;
    QTimer *timer;
    QString ip;
    QString port;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
};

#endif // HEARTBEATTASK_H
