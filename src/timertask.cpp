#include "timertask.h"
#include"operatefile.h"
#include "clientsocket.h"
Timertask::Timertask()
{

}
void Timertask::init()
{
    timer=new QTimer;
    Check_Card_timer=new QTimer;
    Check_Readcard_timer=new QTimer;
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished_timer(QNetworkReply*)),Qt::UniqueConnection);
    //connect(this,SIGNAL(readyRead()),this,SLOT(receiveMsg()));
    connect(this,SIGNAL(readyRead()),this,SLOT(Timertask_GetData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(sendHeartbeatPack()));
    connect(Check_Card_timer,SIGNAL(timeout()),this,SLOT(checkElectricCard()));
    connect(Check_Readcard_timer,SIGNAL(timeout()),this,SLOT(dealReadCardTimeout()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    this->Card_Basemsg={};
    ishascard=false;
    CurrentCardString.clear();
}
void Timertask::StartTimer()
{
    Check_Card_timer->start(750);
}
void Timertask::sendHeartbeatPack()
{
    timer->stop();
    if(QDateTime::currentDateTime().toString("hh")=="01")
    {
        OperateFile::deletefile("log");
        timer->start();

    }
}
void Timertask::dealReadCardTimeout()
{
    Check_Readcard_timer->stop();
    emit hascardmsg("error@读卡超时");
}
void Timertask::receiveHttpMsg(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
    }
    else
    {

    }
    reply->deleteLater();
    OperateFile::deletefile("log");
    timer->start();
}
void Timertask::replyFinished_timer(QNetworkReply *reply)
{

    if(reply->error() == QNetworkReply::NoError)
    {
         QString string;
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Timer task:Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");;
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");

         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETCZCS)//最大充值次数
            {
                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                this->HttpSend(QUrl(QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh)));
                OperateFile::tracelog(QString("timertask Send jsp:\t")+QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh));

            }
           else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETYHYABH)
           {
                //获取到用户档案编号
                this->LK_msg.yhdabh=m_HttpData.at(1);
                if(this->LK_msg.yhdabh=="null")
                {
                    emit hascardmsg("error@"+QString("卡表档案信息中无此用户"));
                    return;
                }
                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                this->HttpSend(QUrl(QString(string).arg(this->LK_msg.yhdabh).arg(QString::number(this->CardType)).arg(this->LK_msg.jlbbh).arg(this->LK_msg.ickid).arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh)));
                OperateFile::tracelog(QString("timertask Send jsp:\t")+QString(string).arg(this->LK_msg.yhdabh).arg(QString::number(this->CardType)).arg(this->LK_msg.jlbbh).arg(this->LK_msg.ickid).arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh));
            }
            //获取到用户信息
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDETAILMSG)
            {
                this->message={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14),m_HttpData.at(15),
                                 m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21),m_HttpData.at(22),m_HttpData.at(23),m_HttpData.at(24),m_HttpData.at(25),m_HttpData.at(26),m_HttpData.at(27),m_HttpData.at(28),m_HttpData.at(29),m_HttpData.at(30),m_HttpData.at(31),
                                 m_HttpData.at(32),m_HttpData.at(33),m_HttpData.at(34),m_HttpData.at(35),m_HttpData.at(36),m_HttpData.at(37),m_HttpData.at(38),m_HttpData.at(39),m_HttpData.at(40),m_HttpData.at(41),m_HttpData.at(42),m_HttpData.at(43),m_HttpData.at(44),m_HttpData.at(45),m_HttpData.at(46),m_HttpData.at(47)};
                emit hascardmsg("ok@");
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETSXFKSTR)
            {
               //checkcard
                this->ReadCard_str=this->ToString();
                this->Card_Basemsg.FileDate1=m_HttpData.at(1);
                this->Card_Basemsg.FileDate2=m_HttpData.at(2);
                this->Card_Basemsg.FileDate3=m_HttpData.at(3);
                this->Card_Basemsg.FileDate4=m_HttpData.at(4);
                if(0==checkCard(this->Card_Basemsg.FileDate1,this->Card_Basemsg.FileDate2,this->Card_Basemsg.FileDate3,this->Card_Basemsg.FileDate4))
                 {
                        if(this->FK_msg.cardtype=="02"||this->FK_msg.cardtype=="04")
                        {
                            //http请求获取最大充值数
                            OperateFile::readiniFile("INTERFACE.DATA","interface/GETCHARGETIMES",string);
                            this->HttpSend(QUrl(QString(string).arg(this->FK_msg.yhdabh).arg(this->FK_msg.kczcs).arg(this->FK_msg.dbbh).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.FileDate1).arg(this->Card_Basemsg.FileDate2).arg(this->Card_Basemsg.FileDate3).arg(this->Card_Basemsg.FileDate4).arg(this->FK_msg.retdata2).arg(this->FK_msg.retdata1)));
                            OperateFile::tracelog("new Send jsp:\t"+QString(string).arg(this->FK_msg.yhdabh).arg(this->FK_msg.kczcs).arg(this->FK_msg.dbbh).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.FileDate1).arg(this->Card_Basemsg.FileDate2).arg(this->Card_Basemsg.FileDate3).arg(this->Card_Basemsg.FileDate4).arg(this->FK_msg.retdata2).arg(this->FK_msg.retdata1));
                        }
                        else
                        {

                            OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                            this->HttpSend(QUrl(QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh)));
                            //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                            OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh));

                        }
                 }
                else
                {
                    //error card
                }
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETSXLKSTR)
            {
                this->ReadCard_str=this->ToString();
                this->Card_Basemsg.FileDate1=m_HttpData.at(1);
                if(0==checkCard(this->Card_Basemsg.FileDate1))
                {
                          //http请求获取用户档案编号

                          OperateFile::readiniFile("INTERFACE.DATA","interface/GETYHDABH",string);
                          this->HttpSend(QUrl(QString(string).arg(this->LK_msg.ickid).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.FileDate1)));
                          OperateFile::tracelog(QString(string).arg(this->LK_msg.ickid).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.FileDate1));
                }
                else
                {
                        //error card
                }
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETZXSTR)
            {
                this->ReadCard_str=this->ToString();

                this->Card_Basemsg.szFileApp=m_HttpData.at(1);
                this->Card_Basemsg.szFileReply=m_HttpData.at(2);
                this->Card_Basemsg.szMoney=m_HttpData.at(3);
                this->Card_Basemsg.szFileParam=m_HttpData.at(4);
                if(0==checkCard(this->Card_Basemsg.szFileApp,this->Card_Basemsg.szFileReply,this->Card_Basemsg.szMoney,this->Card_Basemsg.szFileParam))
                 {
                        if(this->FK_msg.cardtype=="02"||this->FK_msg.cardtype=="04")
                        {
                            //http请求获取最大充值数
                            OperateFile::readiniFile("INTERFACE.DATA","interface/GETCHARGETIMES",string);
                            this->HttpSend(QUrl(QString(string).arg(this->FK_msg.yhdabh).arg(this->FK_msg.kczcs).arg(this->FK_msg.dbbh).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.szFileApp).arg(this->Card_Basemsg.szFileReply).arg(this->Card_Basemsg.szMoney).arg(this->Card_Basemsg.szFileParam).arg(this->FK_msg.retdata2).arg(this->FK_msg.retdata1)));
                            OperateFile::tracelog("new Send jsp:\t"+QString(string).arg(this->FK_msg.yhdabh).arg(this->FK_msg.kczcs).arg(this->FK_msg.dbbh).arg(this->LoginMessage.rybh).arg(this->Card_Basemsg.szFileApp).arg(this->Card_Basemsg.szFileReply).arg(this->Card_Basemsg.szMoney).arg(this->Card_Basemsg.szFileParam).arg(this->FK_msg.retdata2).arg(this->FK_msg.retdata1));

                        }
                        else
                        {

                            OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                            this->HttpSend(QUrl(QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh)));
                            //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                            OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(this->FK_msg.yhdabh).arg("0").arg(this->FK_msg.dbbh).arg("").arg(this->LoginMessage.rybh).arg(this->LoginMessage.gsbh).arg(this->LoginMessage.fgsbh));

                        }
                 }

            }
        }
        else if(base_arg.at(0)=="error")
        {
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETCZCS)
                emit hascardmsg("error@"+QString(m_HttpData.at(1)));
            else
                emit hascardmsg("error@"+QString(m_HttpData.at(1)));
        }
         else
         {
             emit hascardmsg("error@"+QString(m_HttpData.at(1)));
         }
    }
    else
    {
        emit hascardmsg("error@服务暂不可用，请稍后再试");
        OperateFile::tracelog("server refuse");
    }
    reply->deleteLater();
}
void Timertask::replyFinished_detailMsg(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
         OperateFile::tracelog("Timertask: Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
         {
             OperateFile::tracelog("error http argument");
             emit hascardmsg("ignore@");
         }
         if(base_arg.at(0)=="ok"&&QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDETAILMSG)
          {
                    this->message={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14),m_HttpData.at(15),
                                     m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21),m_HttpData.at(22),m_HttpData.at(23),m_HttpData.at(24),m_HttpData.at(25),m_HttpData.at(26),m_HttpData.at(27),m_HttpData.at(28),m_HttpData.at(29),m_HttpData.at(30),m_HttpData.at(31),
                                     m_HttpData.at(32),m_HttpData.at(33),m_HttpData.at(34),m_HttpData.at(35),m_HttpData.at(36),m_HttpData.at(37),m_HttpData.at(38),m_HttpData.at(39),m_HttpData.at(40),m_HttpData.at(41),m_HttpData.at(42),m_HttpData.at(43),m_HttpData.at(44),m_HttpData.at(45),m_HttpData.at(46),m_HttpData.at(47)};
                    emit hascardmsg("ok@");
          }

     else if(base_arg.at(0)=="error")
      {
          emit hascardmsg("warn@"+QString(m_HttpData.at(1)));
      }
    }
    else
    {
        emit hascardmsg("error@服务暂不可用，请到柜台办理业务");
    }
    reply->deleteLater();
}
void Timertask::checkElectricCard()
{
    Check_Card_timer->stop();
    this->sendMsg("Network&9019&1&EC_GetState");

}
void Timertask::Timertask_reCheckCard()
{
    ishascard=false;
}
void Timertask::Timertask_GetData()
{
    QStringList list=this->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }

    if(list.at(3)=="1045")
    {
       emit hascardmsg("error@服务调用异常，请到柜台办理业务");
       return;
    }
    if(list.at(2)=="GetState")
    {
        if(list.at(3)=="0")
        {
            if(ishascard==false)
            {
                emit hascard();
                ishascard=true;
                isSendReadCard=false;
                OperateFile::tracelog("new card insert");
                this->Check_Readcard_timer->start(1000*READCARDTIMEOUT);
                this->CardType=QString(list.at(4)).toInt();
                this->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(this->CardType)));

            }
            else
            {
                isSendReadCard=true;
                this->Check_Readcard_timer->start(1000*READCARDTIMEOUT);
                this->CardType=QString(list.at(4)).toInt();
                this->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(this->CardType)));
                //emit hascardmsg("info@samecard");
            }
        }
        else
        {
           ishascard=false;
           CurrentCardString.clear();
           emit hascardmsg("info@changecard");
        }

    }
    else if(list.at(2)=="ReadCard")
    {
        Check_Readcard_timer->stop();
        if(list.size()<6)
        {
            CurrentCardString.clear();
            emit hascardmsg("info@changecard");
            return;
        }
        if(CurrentCardString==list.at(5))
        {
            emit hascardmsg("info@samecard");
            return;
        }
        if(isSendReadCard==true) emit hascard();
        QString string;
        CurrentCardString=list.at(5);
        switch(QString(list.at(3)).toInt())
        {
            case 0://费控卡
                if(list.size()<9)
                {
                    OperateFile::tracelog("in readcard parement error");
                    emit hascardmsg("info@changecard");
                    return;
                }
                OperateFile::readiniFile("INTERFACE.DATA","interface/GETSXFKSTR",string);
                this->HttpSend(QUrl(QString(string).arg(list.at(5)).arg(list.at(6)).arg(list.at(7)).arg(list.at(8))));
                OperateFile::tracelog(QString(string).arg(list.at(5)).arg(list.at(6)).arg(list.at(7)).arg(list.at(8)));
            break;
            case 1://量控卡
                 if(list.size()<6)
                 {
                    emit hascardmsg("info@changecard");
                    return;
                 }
                 OperateFile::readiniFile("INTERFACE.DATA","interface/GETSXLKSTR",string);
                 this->HttpSend(QUrl(QString(string).arg(list.at(5))));
                 OperateFile::tracelog(QString(string).arg(list.at(5)));
            break;
            case 3://智芯卡
                if(list.size()<11)
                {
                  emit hascardmsg("info@changecard");
                  return;
                }
                this->Card_Basemsg.szCardNum=list.at(5);
                this->Card_Basemsg.szRandFromCard=list.at(6);
                OperateFile::readiniFile("INTERFACE.DATA","interface/GETZXFKSTR",string);
                this->HttpSend(QUrl(QString(string).arg(list.at(7)).arg(list.at(8)).arg(list.at(9)).arg(list.at(10))));
                OperateFile::tracelog(QString(string).arg(list.at(7)).arg(list.at(8)).arg(list.at(9)).arg(list.at(10)));
            break;
            default:
                Check_Card_timer->start(750);
                break;
        }

    }//end of read card
}
int Timertask::checkCard(QString &ickarr)
{
     QStringList list=ickarr.split("/");
     //卡数据格式验证
     if(list.size()<20)
      return -1;
     QString jlbbh=QString(list.at(3)).right(6);
     this->LK_msg={QString(list.at(0)),QString(list.at(1)),QString(QString(list.at(2)).right(14)),QString(list.at(4)),QString(list.at(10)),QString(list.at(17)),QString(list.at(3)),jlbbh,""};
     if(this->CardType==1)
     {
         if(this->LK_msg.CardState=="00")
         {
            emit hascardmsg("error@此卡为空卡,不能购电！");
            return -1;
         }
         else if(this->LK_msg.CardState=="01"||this->LK_msg.CardState=="02"||this->LK_msg.CardState=="04")
          {
             if(this->LK_msg.CardState=="01")
             {
                 if(this->LK_msg.ls_dlzt=="0")
                 {
                     if(this->LK_msg.ls_czcs=="00000000")
                     {
                         emit hascardmsg("error@卡中电量尚未入电表,不能购电！");
                         return -1;
                     }
                 }
             }
             if(this->LK_msg.CardState=="02"||this->LK_msg.CardState=="04")
             {
                 if(this->LK_msg.ls_dlzt=="0")
                 {
                     emit hascardmsg("error@卡中电量尚未入电表,不能购电！");
                     return -1;
                     //return 0;
                 }
             }
          }
         else
         {
             emit hascardmsg("error@请使用开户卡,购电卡进行购电！");
             return -1;
         }
     }
    return 0;
}
/*费控卡参数验证*/
int Timertask::checkCard(QString &data1, QString &data2,QString &data3,QString &data4)
{
    QStringList list1=data1.split("/");
    QStringList list2=data2.split("/");
    QStringList list3=data3.split("/");
    this->FK_msg.cardtype=list1.at(0);
    this->FK_msg.gdcs1=list2.at(10);
    if(this->FK_msg.cardtype=="00")
    {
        emit hascardmsg("error@该卡是新卡，请开卡后进行处理！");
        return -1;
    }
    else if(this->FK_msg.cardtype=="02"||this->FK_msg.cardtype=="04")
    {
        this->FK_msg.khbh=list1.at(10);
        this->FK_msg.dbbh=list1.at(32);
        this->FK_msg.kczcs=list3.at(1);
        this->FK_msg.yhdabh=list1.at(10);
        this->FK_msg.retdata1=list2.at(4);
        this->FK_msg.retdata2=list2.at(5);
    }
    else if(this->FK_msg.cardtype=="01"&&this->FK_msg.gdcs1=="00000000")
    {
            this->FK_msg.yhdabh=list1.at(10);
            this->FK_msg.khbh=list1.at(10);
            this->FK_msg.dbbh=list1.at(32);
    }
    else
    {
        emit hascardmsg("error@请使用开户卡，购电卡进行补卡！");
        return -1;
    }
    if(this->FK_msg.khbh=="")
    {
        emit hascardmsg("error@用户档案编号为空！");
        return -1;
    }
    if(this->FK_msg.khbh=="null")
    {
        emit hascardmsg("error@卡表档案信息中无此用户！");
        return -1;
    }
    return 0;
}
void Timertask::setTimerstate(bool state)
{
    if(state==true)
            Check_Card_timer->start(750);
    else{
           Check_Card_timer->stop();
           Check_Card_timer->isActive();
        }
}
void Timertask::Timertask_cleanMsg()
{
    Check_Card_timer->start();
}
void Timertask::connectError(QAbstractSocket::SocketError socketError)
{
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        emit hascardmsg("error@UMP未启动！");
    }
}
void Timertask::setCardState(const bool &state)
{
    this->ishascard=state;
}
Timertask::~Timertask()
{
    delete timer;
    delete accessManager;
}
