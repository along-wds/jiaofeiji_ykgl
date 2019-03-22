#ifndef COMMONSOCKET_H
#define COMMONSOCKET_H

#include"QtNetwork"

class CommonSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CommonSocket(QObject *parent = 0);
    ~CommonSocket();
    QString ToString();
    void sendMsg(const QString &str);
    void HttpSend(const QUrl &url);
public slots:
        void receiveMsg();
        void hasConnected();
private:
    char *m_receiveData;
    char *m_transmintData;
    QString sendstr;
public:
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
};

#endif // COMMONSOCKET_H
