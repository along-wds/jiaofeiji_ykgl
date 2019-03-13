#include "heartbeattask.h"
#include "operatefile.h"
HeartbeatTask::HeartbeatTask()
{
    OperateFile::readiniFile("config.ini","monitor/IP",ip);
    OperateFile::readiniFile("config.ini","monitor/PORT",port);
}
QDataStream & operator <<(QDataStream &out,Login &LoginMessage)
{
    out<<LoginMessage.fgsbh<<LoginMessage.gsbh<<LoginMessage.rybh\
    <<LoginMessage.rymc<<LoginMessage.sfsjsfjg<<LoginMessage.yyzbh;
    return out;
}

void HeartbeatTask::sendMessage()
{
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<(quint32)0;
    out<<"SPPserver";
    out<<LoginMessage;
    out.device()->seek(0);
    out<<(quint32)(block.size()-sizeof(quint32));
    HeartSocet.write(block,block.size());
}
void HeartbeatTask::recvMessage()
{
    QDataStream in(&HeartSocet);
    quint32 nextBlockSize=0;
    in.setVersion(QDataStream::Qt_5_7);
    if(nextBlockSize==0)
    {
        if(HeartSocet.bytesAvailable()<sizeof(quint64))
            return;

    }
    in >> nextBlockSize;
    if ((quint64)(HeartSocet.bytesAvailable())<nextBlockSize)
    {
        return;
    }
    QString data;
    in>>data;
    recvtime=QDateTime::currentDateTime();
    conn_launch_timer->start(INFOTIME);
}
void HeartbeatTask::init()
{
    conn_launch_timer=new QTimer(this);
    conn_yyt_timer=new QTimer(this);
    check_server_timer=new QTimer(this);

    accessManager = new QNetworkAccessManager(this);
    connect(conn_launch_timer,SIGNAL(timeout()),this,SLOT(sendHeartbeatPack()));
    connect(&HeartSocet,SIGNAL(connected()),this,SLOT(openTimer()));
    connect(&HeartSocet,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    connect(&HeartSocet,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)));
    connect(check_server_timer,SIGNAL(timeout()),this,SLOT(launchHeartBeatServer()));
    connect(conn_yyt_timer,SIGNAL(timeout()),this,SLOT(send2Yyt()));
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    conn_yyt_timer->start(5*60*1000);
    connectToServer();
}
void HeartbeatTask::connectToServer()
{
    HeartSocet.connectToHost(QHostAddress(ip),quint16(port.toInt()));
}
void HeartbeatTask::sendHeartbeatPack()
{
    conn_launch_timer->stop();
    QByteArray block;
    QString clien_name="SPPserver";
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<(quint32)0;
    out<<clien_name;
    out<<LoginMessage;
    out.device()->seek(0);
    out<<(quint32)(block.size()-sizeof(quint32));
    HeartSocet.write(block,block.size());
    HeartSocet.flush();
}
void HeartbeatTask::send2Yyt()
{
    conn_yyt_timer->stop();
    QString urlString,devTid;
    OperateFile::readiniFile("INTERFACE.DATA","YYT/SENDSTATE",urlString);
    OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",devTid);
    HttpSend(QUrl(QString(urlString).arg(devTid)));
    OperateFile::tracelog("http send:"+QString(urlString).arg(devTid));

}
void HeartbeatTask::openTimer()
{
    check_server_timer->stop();
    conn_launch_timer->start(INFOTIME);

}
void HeartbeatTask::launchHeartBeatServer()
{
    check_server_timer->stop();
    connectToServer();
}
void HeartbeatTask::connectError(QAbstractSocket::SocketError socketError)
{
    if(socketError==QAbstractSocket::ConnectionRefusedError||socketError==QAbstractSocket::RemoteHostClosedError||socketError==QAbstractSocket::HostNotFoundError)
    {
        OperateFile::tracelog("connect to monitor fail");
        //QProcess::startDetached("Launch.exe");
        check_server_timer->start(20000);
    }
}
void HeartbeatTask::HttpSend(const QUrl &url)
{
    request.setUrl(url);
    reply = accessManager->get(request);
}
void HeartbeatTask::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
         char m_receivedata[512];
         memset(m_receivedata,0,512);
         reply->read(m_receivedata,512);
         OperateFile::tracelog("recv from ttyzhfu:"+QString::fromLocal8Bit(m_receivedata));
    }
    else
    {
       OperateFile::tracelog("=========connect to yytzhfupt failed=========");
    }
    conn_yyt_timer->start(5*60*1000);
    reply->deleteLater();
}
