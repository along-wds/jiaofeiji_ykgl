#include "heartbeattask.h"

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
    this->write(block,block.size());
}
void HeartbeatTask::recvMessage()
{
    QDataStream in(this);
    quint32 nextBlockSize=0;
    in.setVersion(QDataStream::Qt_5_7);
    if(nextBlockSize==0)
    {
        if(this->bytesAvailable()<sizeof(quint64))
            return;

    }
    in >> nextBlockSize;
    if ((quint64)(this->bytesAvailable())<nextBlockSize)
    {
        return;
    }
    QString data;
    in>>data;
    qDebug()<<"recv data:"<<data;
    recvtime=QDateTime::currentDateTime();
    timer->start();
}
void HeartbeatTask::init()
{
    timer=new QTimer;
    timer_check_server=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(sendHeartbeatPack()));
    connect(this,SIGNAL(connected()),this,SLOT(openTimer()));
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)));
    connect(timer_check_server,SIGNAL(timeout()),this,SLOT(launchHeartBeatServer()));
    connectToServer();
}
void HeartbeatTask::connectToServer()
{
    this->connectToHost(QHostAddress(ip),quint16(port.toInt()));
}
void HeartbeatTask::sendHeartbeatPack()
{
    qDebug()<<"send message";
    QByteArray block;
    QString clien_name="SPPserver";
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<(quint32)0;
    out<<clien_name;
    out<<LoginMessage;
    out.device()->seek(0);
    out<<(quint32)(block.size()-sizeof(quint32));
    this->write(block,block.size());
    this->flush();
}
void HeartbeatTask::openTimer()
{
    timer_check_server->stop();
    timer->start(INFOTIME);
}
void HeartbeatTask::launchHeartBeatServer()
{
     connectToServer();
}
void HeartbeatTask::connectError(QAbstractSocket::SocketError socketError)
{

    //qDebug()<<socketError;
    if(socketError==QAbstractSocket::ConnectionRefusedError||socketError==QAbstractSocket::RemoteHostClosedError||socketError==QAbstractSocket::HostNotFoundError)
    {
        OperateFile::tracelog("connect to monitor fail");
        /*QProcess::startDetached("Launch.exe");
        timer_check_server->start(10000);*/
    }
}
void HeartbeatTask::HttpSend(const QUrl &url)
{
    request.setUrl(url);
    reply = accessManager->get(request);
}
