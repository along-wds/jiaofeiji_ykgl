#include "readcard.h"
#include "ui_readcard.h"
#include"cashpay.h"
#include"unionpay.h"
#include"msgconfirm.h"
#include"homepage.h"
#include"lookup.h"
#include"manager.h"
#include "choiceuser.h"
#include"webpage.h"
#include "identifycode.h"
readcard::readcard(QWidget *parent) :
     QWidget(parent),
    ui(new Ui::readcard),m_currentImageindex(0),opacityinterval(0.05)
{
   ui->setupUi(this);
   ui->frame->setObjectName("frame2");
   ui->frame_5->setObjectName("frame5");
   ui->frame->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/10.png);}");
   ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");

   ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
   ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                   "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
   ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                     "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
   ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                     "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
   ui->lineEdit->setStyleSheet("font-size: 25px;");
   ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
   //调色板
   QPalette lcdpat =ui->lcdNumber->palette();
   /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
   lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
   //设置当前窗口的调色板

   ui->lcdNumber->setPalette(lcdpat);
   ui->lineEdit->setMaxLength(18);

   tm_switch = new QTimer(this);
   tm_start=new QTimer(this);
   Check_Readcard_timer= new QTimer(this);
   pictureeffect = new QGraphicsOpacityEffect();
   connect(tm_switch,SIGNAL(timeout()),this,SLOT(loopPicture()));
   mv_media = new QMovie(this);
   ui->label_4->setMovie(mv_media);
   mv_media->setFileName(":/image/picture/qietu/info_xia3.png");
   mv_media->start();
   opacity = 1.0;//透明度值
   switchcase = 0;//淡入淡出中间状态标志
   imagesList.append(":/image/picture/qietu/info_xia3.png");
   imagesList.append(":/image/picture/qietu/info_xia4.jpg");
   //imagesList.append(":/image/picture/qietu/info_xia5.jpg");


}
void readcard::init()
{
    if(socket->IsPurchase==true)
    {
        ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                        "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
        ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                          "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    }
    else
    {
        ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                        "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
        ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search+.png);}"
                                          "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    }
    lkid=false;
    fkid=false;
    Timeoutflag=false;
    ui->pushButton_Ok->setEnabled(true);
    ui->pushButton_readCard->setEnabled(true);
    ui->pushButton_search->setEnabled(true);
    ui->pushButton_purchase->setEnabled(true);
    ui->pushButton_home->setEnabled(true);
    readcard_button=false;
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText("户号/手机号/身份证号");
    Replytimes=0;
    CardType=2;
    connect(tm_start,SIGNAL(timeout()),this,SLOT(startSwitch()),Qt::UniqueConnection);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readCard_GetData()),Qt::UniqueConnection);//?写在这里才生效，有待修改
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)),Qt::DirectConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    connect(Check_Readcard_timer,SIGNAL(timeout()),this,SLOT(dealReadCardTimeout()));
    socket->CardType=2;
    startTimer();
}
readcard::~readcard()
{
    delete ui;
}
void readcard::loopPicture()
{
    if (opacity > 0.0)
    {

        if (switchcase == 0)
        {
            opacity -= opacityinterval;
        }
        else
        {
            opacity += opacityinterval;
        }
        setPictureOpacity();
        if (opacity == 1.0)
        {
            tm_switch->stop();
            switchcase = 0;

        }
    }
    else
    {

            m_currentImageindex++;
            if(m_currentImageindex==imagesList.size())
            {
                m_currentImageindex=0;
            }
            mv_media->stop();
            mv_media->setFileName(imagesList.at(m_currentImageindex));
            mv_media->start();
            switchcase = 1;
            opacity += opacityinterval;
            setPictureOpacity();
    }

}
void readcard::startSwitch()
{

    tm_switch->start(30);
}
void readcard::setPictureOpacity()
{
    //*淡入淡出效果*************************************************//
    pictureeffect->setOpacity(opacity);
    ui->label_4->setGraphicsEffect(pictureeffect);
}
void readcard::on_pushButton_clicked()
{
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}
void readcard::on_pushButton_readCard_clicked()
{
    startTimer();
    Timeoutflag=true;
    messagebox->displayWithoutBlock(this,"正在读卡");
    Check_Readcard_timer->start(1000*READCARDTIMEOUT);
    socket->sendMsg("Network&9019&1&EC_GetState");
}
void readcard::readCard_GetData()
{

    Check_Readcard_timer->stop();
    //if(!(this->ToString().contains("EC_GetState")||this->ToString().contains("EC_ReadCard")))
    OperateFile::tracelog("TCP receive:"+socket->ToString());
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }
    if(list.at(3)=="1045")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务调用异常，请到柜台办理业务");
        return;
    }
    else if(list.at(3)=="1000"||list.at(3)=="1001")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"请正确插入购电卡");
        return;
    }
    if(list.at(2)=="GetState")
    {
        if(list.at(3)!="0")
        {
           // 获取卡类型失败
            messagebox->closeDialog();
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法识别的卡");
            return;
        }
        else
        {
            CardType=QString(list.at(4)).toInt();
            socket->CardType=CardType;
            Check_Readcard_timer->start(1000*READCARDTIMEOUT);
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(CardType)));
        }

    }
    if(list.at(2)=="ReadCard")
    {
        QString string;
        Timeoutflag=false;
        switch(QString(list.at(3)).toInt())
        {
        case 0://费控卡
            if(list.size()<9)
            {
                OperateFile::tracelog("in readcard parement error");
                messagebox->closeDialog();
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"读卡失败");
                return;
            }
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETSXFKSTR",string);
            socket->HttpSend(QUrl(QString(string).arg(list.at(5)).arg(list.at(6)).arg(list.at(7)).arg(list.at(8))));
            OperateFile::tracelog(QString(string).arg(list.at(5)).arg(list.at(6)).arg(list.at(7)).arg(list.at(8)));
            break;
        case 1://量控卡
              if(list.size()<6)
              {
                  OperateFile::tracelog("in readcard parement error");
                  messagebox->closeDialog();
                  QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                  messagebox->displayWithoutButton(this,"读卡失败");
                  return;
              }
              OperateFile::readiniFile("INTERFACE.DATA","interface/GETSXLKSTR",string);
              socket->HttpSend(QUrl(QString(string).arg(list.at(5))));
              OperateFile::tracelog(QString(string).arg(list.at(5)));
            break;
         case 3:
            if(list.size()<11)
            {
                OperateFile::tracelog("in readcard parement error");
                messagebox->closeDialog();
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"读卡失败");
                return;
            }
            socket->Card_Basemsg.szCardNum=list.at(5);
            socket->Card_Basemsg.szRandFromCard=list.at(6);
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETZXFKSTR",string);
            socket->HttpSend(QUrl(QString(string).arg(list.at(7)).arg(list.at(8)).arg(list.at(9)).arg(list.at(10))));
            OperateFile::tracelog(QString(string).arg(list.at(7)).arg(list.at(8)).arg(list.at(9)).arg(list.at(10)));
            break;
        default:
            messagebox->closeDialog();
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法识别的购电卡");
            OperateFile::tracelog("in readcard read error:"+socket->ToString());
            break;

    }
    }
}
void readcard::dealReadCardTimeout()
{
    Check_Readcard_timer->stop();
    messagebox->closeDialog();
    QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
    messagebox->displayWithoutButton(this,"读卡超时，请稍后再试");
}
void readcard::readCard_DealData()
{

}
void readcard::readCard_reshow()
{
    init();
}
/*量控卡参数验证*/
int readcard::checkCard(QString &ickarr)
{
     QStringList list=ickarr.split("/");
     /*卡数据格式验证*/
     if(list.size()<20)
      return -1;
     QString jlbbh=QString(list.at(3)).right(6);
     socket->LK_msg={QString(list.at(0)),QString(list.at(1)),QString(QString(list.at(2)).right(14)),QString(list.at(4)),QString(list.at(10)),QString(list.at(17)),QString(list.at(3)),jlbbh,""};
     if(CardType==1)
     {
         if(socket->LK_msg.CardState=="00")
         {
            messagebox->closeDialog();
            QTimer::singleShot(4000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"此卡为空卡,不能购电！");
            return -1;
         }
         else if(socket->LK_msg.CardState=="01"||socket->LK_msg.CardState=="02"||socket->LK_msg.CardState=="04")
          {
             if(socket->LK_msg.CardState=="01")
             {
                 if(socket->LK_msg.ls_dlzt=="0")
                 {
                     if(socket->LK_msg.ls_czcs=="00000000")
                     {
                         qDebug()<<"11111111";
#ifndef DEBUG
                         messagebox->closeDialog();
                         QTimer::singleShot(4000, messagebox, SLOT(closeDialog()));
                         messagebox->displayWithoutButton(this,"卡中电量尚未入电表,不能购电！");
                         return -1;
#else
                         return 0;
#endif
                     }

                 }
             }
             if(socket->LK_msg.CardState=="02"||socket->LK_msg.CardState=="04")
             {
                 if(socket->LK_msg.ls_dlzt=="0")//实际socket->LK_msg.ls_dlzt=="0"，调试用
                 {
                     messagebox->closeDialog();
                     QTimer::singleShot(4000, messagebox, SLOT(closeDialog()));
                     messagebox->displayWithoutButton(this,"卡中电量尚未入电表,不能购电！");
                     return -1;
                 }
             }
          }
         else
         {
             messagebox->closeDialog();
             QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
             messagebox->displayWithoutButton(this,"请使用开户卡,购电卡进行购电！");
             return -1;
         }
     }
    return 0;
}
/*费控卡参数验证*/
int readcard::checkCard(QString &data1, QString &data2,QString &data3,QString &data4)
{
    QStringList list1=data1.split("/");
    QStringList list2=data2.split("/");
    QStringList list3=data3.split("/");
    socket->FK_msg.cardtype=list1.at(0);
    socket->FK_msg.gdcs1=list2.at(10);
    qDebug()<<"cardtype:"<<socket->FK_msg.cardtype;
    if(socket->FK_msg.cardtype=="00")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"该卡是新卡，请开卡后进行处理！");
        return -1;
    }
    else if(socket->FK_msg.cardtype=="02"||socket->FK_msg.cardtype=="04")
    {
        socket->FK_msg.khbh=list1.at(10);
        socket->FK_msg.dbbh=list1.at(32);
        socket->FK_msg.kczcs=list3.at(1);
        socket->FK_msg.yhdabh=list1.at(10);
        socket->FK_msg.retdata1=list2.at(4);
        socket->FK_msg.retdata2=list2.at(5);
    }
    else if(socket->FK_msg.cardtype=="01"&&socket->FK_msg.gdcs1=="00000000")
    {
            socket->FK_msg.yhdabh=list1.at(10);
            socket->FK_msg.khbh=list1.at(10);
            socket->FK_msg.dbbh=list1.at(32);
    }
    else
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"请使用开户卡，购电卡进行补卡！");
        return -1;
    }
    if(socket->FK_msg.khbh=="")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"用户档案编号为空！");
        return -1;
    }
    if(socket->FK_msg.khbh=="null")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"卡表档案信息中无此用户！");
        return -1;
    }
    readcard_button=true;
    return 0;
}
void readcard::replyFinished(QNetworkReply *reply)
{
    Replytimes++;
    if(reply->error() == QNetworkReply::NoError)
    {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         reply->read(m_receivedata,4096);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
         QStringList base_arg;
         QString string;
         qDebug()<<"m_httpdata.size:"<<m_HttpData.size()<<m_HttpData;
         if(m_HttpData.size()==1)
         {
             m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
             base_arg=QString(m_HttpData.at(0)).split("!*!");

             if(base_arg.at(0)=="ok")
             {
                 if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
                 {
                     OperateFile::tracelog("error argument");
                     return;
                 }
                if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETCZCS)
                {
                    fkid=true;
                    on_pushButton_Ok_clicked();
                }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETYHYABH)
                {
                    socket->LK_msg.yhdabh=m_HttpData.at(1);
                    ui->lineEdit->setText(socket->LK_msg.yhdabh);
                    lkid=true;
                    on_pushButton_Ok_clicked();
                }

                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDETAILMSG)
                 {
                     messagebox->closeDialog();
                     socket->message={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14),m_HttpData.at(15),
                                            m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21),m_HttpData.at(22),m_HttpData.at(23),m_HttpData.at(24),m_HttpData.at(25),m_HttpData.at(26),m_HttpData.at(27),m_HttpData.at(28),m_HttpData.at(29),m_HttpData.at(30),m_HttpData.at(31),
                                            m_HttpData.at(32),m_HttpData.at(33),m_HttpData.at(34),m_HttpData.at(35),m_HttpData.at(36),m_HttpData.at(37),m_HttpData.at(38),m_HttpData.at(39),m_HttpData.at(40),m_HttpData.at(41),m_HttpData.at(42),m_HttpData.at(43),m_HttpData.at(44),m_HttpData.at(45),m_HttpData.at(46),m_HttpData.at(47)};
                     socket->Isjumpreadcard=true;
                     timertask->CardType=socket->CardType;
                     timertask->ReadCard_str=socket->ReadCard_str;
                     timertask->Card_Basemsg=socket->Card_Basemsg;
                     timertask->LK_msg=socket->LK_msg;
                     timertask->FK_msg=socket->FK_msg;
                     timertask->message=socket->message;
                     timertask->setCardState(true);
                     if(socket->IsPurchase==true)
                     {
                           if(OperateFile::ui_msgconfirm==0)
                           {
                              OperateFile::ui_msgconfirm =new MsgConfirm();
                           }
                           disconnectslots();
                           /*********************待调整**********************************/
                           if(socket->message.fffs!="01" && socket->message.fffs!="03" )
                           {
                               if(socket->message.khbz=="0"){
                                   QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                                   messagebox->displayWithoutButton(this,"您还未开户，请去柜台处理！");
                                   return;
                                   }
                               else{
                                       //页面显示调整金额和您的缴费金额应大于 显示
                                       OperateFile::ui_msgconfirm->showTzje();
                                   }
                           }
                           //普通户
                           else
                           {
                                OperateFile::ui_msgconfirm->hideTzje();
                           }
                           ui->lineEdit->setText(socket->FK_msg.yhdabh);
                           socket->effect->begin(this, OperateFile::ui_msgconfirm,LEFTTORIGHT,NONE,HIDE);
                           OperateFile::ui_msgconfirm->init();
                    }
                     else if(socket->IsPurchase==false)
                     {

                         if(OperateFile::ui_lookup==0)
                         {
                             OperateFile::ui_lookup=new Lookup();
                         }
                        disconnectslots();
                        socket->effect->begin(this,OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
                        OperateFile::ui_lookup->init();
                     }
                }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETSXFKSTR)
                {
                   //checkcard
                    socket->ReadCard_str=socket->ToString();
                    socket->Card_Basemsg.FileDate1=m_HttpData.at(1);
                    socket->Card_Basemsg.FileDate2=m_HttpData.at(2);
                    socket->Card_Basemsg.FileDate3=m_HttpData.at(3);
                    socket->Card_Basemsg.FileDate4=m_HttpData.at(4);
                    if(0==checkCard(socket->Card_Basemsg.FileDate1,socket->Card_Basemsg.FileDate2,socket->Card_Basemsg.FileDate3,socket->Card_Basemsg.FileDate4))
                     {
                            if(socket->FK_msg.cardtype=="02"||socket->FK_msg.cardtype=="04")
                            {
#ifdef DEBUG
                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
                                //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                                OperateFile::tracelog(QString("readcard fk Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));
#else
                                //http请求获取最大充值数
                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETCHARGETIMES",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.kczcs).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.FileDate1).arg(socket->Card_Basemsg.FileDate2).arg(socket->Card_Basemsg.FileDate3).arg(socket->Card_Basemsg.FileDate4).arg(socket->FK_msg.retdata2).arg(socket->FK_msg.retdata1)));
                                OperateFile::tracelog("new Send jsp:\t"+QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.kczcs).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.FileDate1).arg(socket->Card_Basemsg.FileDate2).arg(socket->Card_Basemsg.FileDate3).arg(socket->Card_Basemsg.FileDate4).arg(socket->FK_msg.retdata2).arg(socket->FK_msg.retdata1));
#endif
                            }
                            else
                            {

                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
                                //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                                OperateFile::tracelog(QString("readcard fk Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));

                            }
                     }
                    else
                    {
                        //error card
                    }
                }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETSXLKSTR)
                {
                    socket->ReadCard_str=socket->ToString();
                    socket->Card_Basemsg.FileDate1=m_HttpData.at(1);
                    if(0==checkCard(socket->Card_Basemsg.FileDate1))
                    {
                              //http请求获取用户档案编号

                              OperateFile::readiniFile("INTERFACE.DATA","interface/GETYHDABH",string);
                              socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.ickid).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.FileDate1)));
                              OperateFile::tracelog(QString(string).arg(socket->LK_msg.ickid).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.FileDate1));
                    }
                    else
                    {
                            //error card
                    }
                }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETZXSTR)
                {
                    socket->ReadCard_str=socket->ToString();
                    socket->Card_Basemsg.szFileApp=m_HttpData.at(1);
                    socket->Card_Basemsg.szFileReply=m_HttpData.at(2);
                    socket->Card_Basemsg.szMoney=m_HttpData.at(3);
                    socket->Card_Basemsg.szFileParam=m_HttpData.at(4);
                    if(0==checkCard(socket->Card_Basemsg.szFileApp,socket->Card_Basemsg.szFileReply,socket->Card_Basemsg.szMoney,socket->Card_Basemsg.szFileParam))
                     {
                            if(socket->FK_msg.cardtype=="02"||socket->FK_msg.cardtype=="04")
                            {
#ifdef DEBUG
                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
                                //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                                OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));
#else
                                //http请求获取最大充值数
                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETCHARGETIMES",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.kczcs).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.szFileApp).arg(socket->Card_Basemsg.szFileReply).arg(socket->Card_Basemsg.szMoney).arg(socket->Card_Basemsg.szFileParam).arg(socket->FK_msg.retdata2).arg(socket->FK_msg.retdata1)));
                                OperateFile::tracelog("readcard zx Send jsp:\t"+QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.kczcs).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg(socket->Card_Basemsg.szFileApp).arg(socket->Card_Basemsg.szFileReply).arg(socket->Card_Basemsg.szMoney).arg(socket->Card_Basemsg.szFileParam).arg(socket->FK_msg.retdata2).arg(socket->FK_msg.retdata1));
#endif
                            }
                            else
                            {

                                OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
                                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
                                //qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg("0").arg(socket->Card_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh);
                                OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));

                            }
                     }
                }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDXYZM)
                {
                    if(m_HttpData.at(1)=="1")
                    {
                        messagebox->closeDialog();
                        disconnectslots();
                        OperateFile::ui_identifycode=new IdentifyCode(ui->lineEdit->text(),CurrentCode);
                        OperateFile::ui_identifycode->setAttribute(Qt::WA_DeleteOnClose);
                        socket->effect->begin(this,OperateFile::ui_identifycode,LEFTTORIGHT,NONE,HIDE);
                        OperateFile::ui_identifycode->init();
                    }
                    else
                    {
                        QString GetInfoUrl;
                        OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",GetInfoUrl);
                        socket->HttpSend(QUrl(QString(GetInfoUrl).arg(ui->lineEdit->text()).arg("2").arg("").arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
                        OperateFile::tracelog(QString("Send jsp:\t")+QString(GetInfoUrl).arg(ui->lineEdit->text()).arg("2").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));

                    }
                }

            }

         else if(base_arg.at(0)=="error")
          {

                   messagebox->closeDialog();
                   OperateFile::tracelog("in readcard:"+QString(m_HttpData.at(1)));
                   QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                   messagebox->displayWithoutButton(this,m_HttpData.at(1));

               ui->pushButton_Ok->setEnabled(true);
               ui->pushButton_readCard->setEnabled(true);
          }
         }
         else  //多表用户
         {

             disconnectslots();
             ChoiceUser *choiceuser=new ChoiceUser(m_HttpData);
             choiceuser->setAttribute(Qt::WA_DeleteOnClose);
             socket->effect->begin(this,choiceuser,LEFTTORIGHT,NONE,HIDE);
             choiceuser->init();
         }
    }
    else
    {
        ui->pushButton_Ok->setEnabled(true);
        ui->pushButton_readCard->setEnabled(true);
        messagebox->closeDialog();
        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
    }
    reply->deleteLater();
}

void readcard::on_pushButton_1_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"1");
}

void readcard::on_pushButton_2_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"2");
}

void readcard::on_pushButton_3_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"3");
}

void readcard::on_pushButton_4_clicked()
{   mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"4");
}

void readcard::on_pushButton_5_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"5");
}

void readcard::on_pushButton_6_clicked()
{
   mousePressEvent(NULL);
   ui->lineEdit->setText(ui->lineEdit->text()+"6");
}

void readcard::on_pushButton_7_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"7");
}

void readcard::on_pushButton_8_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"8");
}

void readcard::on_pushButton_9_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"9");
}

void readcard::on_pushButton_0_clicked()
{
     mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"0");
}
void readcard::on_pushButton_00_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"x");
}
void readcard::on_pushButton_BackSapce_clicked()
{
    mousePressEvent(NULL);
    if(ui->lineEdit->text().isEmpty())
        return;
    QString text=ui->lineEdit->text();
    text.chop(1);
    ui->lineEdit->setText(text);
}
void readcard::on_pushButton_Reset_clicked()
{
    mousePressEvent(NULL);
    if(ui->lineEdit->text().isEmpty())
        return;
    ui->lineEdit->setText("");
}
/*void readcard::on_pushButton__readCard_clicked()
{
    on_pushButton_Ok_clicked();
}*/
/*点击确认按钮*/
void readcard::on_pushButton_Ok_clicked()
{

    startTimer();
    if(readcard_button==false&&ui->lineEdit->text()=="")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"请先输入户号或点击读卡按钮");
        return;
    }
    /***********************
     * 将读卡按钮置为不可用
     **************************/
    messagebox->closeDialog();
    messagebox->displayWithoutBlock(this,"正在获取信息……");
    ui->pushButton_Ok->setEnabled(false);
    ui->pushButton_readCard->setEnabled(false);
    /**************购电***************/
        QString string;
        OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",string);
        /***********无卡*****************/
        if(CardType==2)
        {
            QString CheckConentUrl;
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETDXYZM",CheckConentUrl);
            OperateFile::getIdentifyCode(CurrentCode,6);
            socket->HttpSend(QUrl(QString(CheckConentUrl).arg(ui->lineEdit->text()).arg(CurrentCode)));
            OperateFile::tracelog(QString("Send jsp:\t")+QString(CheckConentUrl).arg(ui->lineEdit->text()).arg(CurrentCode));
        }
        /**************费控有卡*********************/
        else if((CardType==0||CardType==3)&&fkid==true)
        {
            socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
            OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg("0").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));
        }
        /***********量控有卡***********/
        else if(CardType==1&&lkid==true)
        {
            socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.yhdabh).arg(QString::number(CardType)).arg("").arg(socket->LK_msg.ickid).arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
            OperateFile::tracelog(QString(string).arg(socket->LK_msg.yhdabh).arg(QString::number(CardType)).arg("").arg(socket->LK_msg.ickid).arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));
        }


}
void readcard::connectError(QAbstractSocket::SocketError socketError)
{
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        messagebox->closeDialog();
        disconnectslots();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"UMP未启动！");
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE);
        OperateFile::ui_homepage->init();
    }
}
void readcard::getCardtype()
{
    if(CardType!=2)
        timer->stop();
    socket->sendMsg("Network&9019&1&EC_GetState");
    readcard_button=true;
}
void readcard::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_10->show();
}
void readcard::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->lcdNumber->hide();
   ui->label_10->hide();
   emit start(MSEC);
}
void readcard::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_10->hide();
    emit start(MSEC);
    tm_start->start(3000);
}
void readcard::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       messagebox->closeDialog();
       if(Timeoutflag==true)
       {
           OperateFile::tracelog("in readcard:quest timeout");
           QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"请求超时，请到柜台办理业务");

       }
       disconnectslots();
       socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,NEIYE);
       OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void readcard::disconnectslots()
{
    emit stop();
    tm_start->stop();
    disconnect(tm_start,SIGNAL(timeout()),this,SLOT(startSwitch()));
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(readCard_GetData()));
    disconnect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)));
    disconnect(Check_Readcard_timer,SIGNAL(timeout()),this,SLOT(dealReadCardTimeout()));
}

void readcard::on_lineEdit_textChanged(const QString &arg1)
{
 /*if(arg1=="")
 {
     ui->pushButton_Ok->setStyleSheet("border-image: url(:/image/picture/qietu/06-.png);");
     ui->pushButton_Ok->setEnabled(false);
 }
 else{
        ui->pushButton_Ok->setStyleSheet("border-image: url(:/image/picture/qietu/06.png);");
        ui->pushButton_Ok->setEnabled(true);
     }*/
}
void readcard::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}

void readcard::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=true;
    OperateFile::ui_readcard->init();
}

void readcard::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=false;
    OperateFile::ui_readcard->init();

}

void readcard::on_pushButton_public_clicked()
{
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_webpage,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_webpage->init();

}
