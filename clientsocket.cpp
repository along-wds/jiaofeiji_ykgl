#include<clientsocket.h>
#include<qdebug.h>
#include"homepage.h"
ClientSocket::ClientSocket(QObject *parent)
: CommonSocket(parent)
{
    //connect(this,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
    /*m_receiveData=new char[2048];
    m_transmintData=new char[2048];
    memset(m_receiveData,0,2048);
    accessManager = new QNetworkAccessManager(this);
    connect(this,SIGNAL(connected()),this,SLOT(hasConnected()));*/
    effect=new Effect;
    player=new QMediaPlayer();
    unionThread=new Unionthread;
    thread = new QThread;
    //Multmessagebox=new MulitDialog();
}
/*void ClientSocket::receiveMsg()
{
    this->read(m_receiveData,2048);
    //if(!sendstr.contains("EC_GetState"))
       // OperateFile::tracelog("TCP receive:"+QString::fromLocal8Bit(m_receiveData));
    memcpy(m_transmintData,m_receiveData,2048);
    memset(m_receiveData,0,2048);
    flag=true;
}
void ClientSocket::sendMsg(const QString &str)
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
QString ClientSocket::ToString()
{
    QString str=QString::fromLocal8Bit(m_transmintData);
    return str;
}*/
ClientSocket::~ClientSocket()
{
    delete effect;
    delete player;

}
/*void ClientSocket::hasConnected()
{
    QByteArray ba = sendstr.toLocal8Bit();
    char *p=ba.data();
    this->write(p);
    this->flush();
}
void ClientSocket::HttpSend(const QUrl &url)
{
    is_wait_http_resp=true;
    request.setUrl(url);
    reply = accessManager->get(request);
}*/
