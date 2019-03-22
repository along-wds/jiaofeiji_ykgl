#include "commonsocket.h"
#include "operatefile.h"
CommonSocket::CommonSocket(QObject *parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
    m_receiveData=new char[2048];
    m_transmintData=new char[2048];
    memset(m_receiveData,0,2048);
    accessManager = new QNetworkAccessManager(this);
    connect(this,SIGNAL(connected()),this,SLOT(hasConnected()));
}
CommonSocket::~CommonSocket()
{
    delete []m_receiveData;
    delete []m_transmintData;
}
void CommonSocket::receiveMsg()
{
    this->read(m_receiveData,2048);
    memcpy(m_transmintData,m_receiveData,2048);
    memset(m_receiveData,0,2048);
}
void CommonSocket::sendMsg(const QString &str)
{
    QString IP,PORT;
    sendstr=str;
    OperateFile::readiniFile("INTERFACE.DATA","Option/IP",IP);
    OperateFile::readiniFile("INTERFACE.DATA","Option/PORT",PORT);
    if(!(sendstr.contains("EC_GetState")||sendstr.contains("EC_ReadCard")))
        OperateFile::tracelog("TCP send:"+sendstr);
    this->abort();
    this->connectToHost("127.0.0.1",9002);
}
QString CommonSocket::ToString()
{
    QString str=QString::fromLocal8Bit(m_transmintData);
    return str;
}
void CommonSocket::hasConnected()
{
    QByteArray ba = sendstr.toLocal8Bit();
    char *p=ba.data();
    this->write(p);
    this->flush();
}
void CommonSocket::HttpSend(const QUrl &url)
{
    request.setUrl(url);
    reply = accessManager->get(request);
}
