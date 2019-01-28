#include "writecard.h"
#include "ui_writecard.h"
#include"homepage.h"
#include"writecardlist.h"
#include"QMessageBox"
#include"first.h"
#include"manager.h"
#include"readcard.h"
extern Manager *ui_manager;
extern First ui_first;
extern HomePage *homepage;
Writecard::Writecard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Writecard)
{
    times=0;
    ui->setupUi(this);
    ui->frame_2->setObjectName("frame2");
    ui->frame_3->setObjectName("frame3");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/xiadaohang.png);}");
    ui->frame_3->setStyleSheet("QFrame#frame3{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    ui->pushButton_backHomePage_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_03.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_03.png);}");
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_05.png);}");
    ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_09.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_09.png);}");
    ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_07.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_07.png);}");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);

}
void Writecard::init()
{
    Timeoutflag=false;
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label_10->hide();
    ui->pushButton_readcard->setEnabled(true);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket,SIGNAL(readyRead()), this,SLOT(writecard_GetData()),Qt::UniqueConnection);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    startTimer();
}
Writecard::~Writecard()
{
    delete ui;
}
void Writecard::writecard_GetData()
{
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
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(CardType)));
        }

    }
    if(list.at(2)=="ReadCard")
    {
        switch(QString(list.at(3)).toInt())
        {
        case 0://费控卡
            if(list.size()<10)
            {
                OperateFile::tracelog("in readcard parement error");
                return;
            }
                socket->ReadCard_str=socket->ToString();
                socket->Card_Basemsg.FileDate1=list.at(5);
                socket->Card_Basemsg.FileDate2=list.at(6);
                socket->Card_Basemsg.FileDate3=list.at(7);
                socket->Card_Basemsg.FileDate4=list.at(8);
               if(0==checkCard(socket->Card_Basemsg.FileDate1,socket->Card_Basemsg.FileDate2,socket->Card_Basemsg.FileDate3,socket->Card_Basemsg.FileDate4))
                 {
                   QString string;
                   OperateFile::readiniFile("INTERFACE.DATA","interface/GETFKBK",string);
                   socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(FileData1).arg(FileData2).arg(FileData3).arg(FileData4)));
                 }
                else
                {
                    //error card
                }
            break;
        case 1://量控卡
              if(list.size()<6)
                return;
              socket->ReadCard_str=socket->ToString();
              FileData1=list.at(5);
              if(socket->IsPurchase==false)
              {
                  //获取客户信息
                  //ui->lineEdit->setText(socket->LK_msg.ickid);//显示卡号
                  QString string;
                  OperateFile::readiniFile("INTERFACE.DATA","interface/GETYHDABH",string);
                  socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.ickid).arg(FileData1)));
              }
              else
              {
                  // 卡参数验证
                  if(0==checkCard(FileData1))
                   {
                      QString string;
                      OperateFile::readiniFile("INTERFACE.DATA","interface/GETLKBK",string);
                      socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.ickid).arg(socket->LK_msg.jlbbh).arg(FileData1)));
                      //qDebug()<<QString((string).arg(socket->Lk_msg.ickid).arg(socket->Lk_msg.jlbbh).arg(FileData1));
                   }
                  else
                  {
                      //error card
                  }
                }
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
/**/
void Writecard::writecard_reshow()
{
   connect(socket,SIGNAL(readyRead()),this,SLOT(readCard_GetData()));
   connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
   this->show();
}
/*量控卡参数验证*/
int Writecard::checkCard(QString &ickarr)
{

     QStringList list=ickarr.split("/");
     /*卡数据格式验证*/
     if(list.size()<20)
      return -1;
     QString jlbbh=QString(list.at(3)).right(6);
     socket->LK_msg={QString(list.at(0)),QString(list.at(1)),QString(list.at(2)).right(14),QString(list.at(4)),QString(list.at(10)),QString(list.at(17)),QString(list.at(3)),jlbbh,""};
     if(CardType==1)
     {
         if(socket->LK_msg.CardState=="00")
         {
            QMessageBox msgbox(QMessageBox::Warning,"未识别的卡","       此卡为空卡，不能购电！           ",0,this);
            msgbox.addButton("确认", QMessageBox::YesRole);
            msgbox.exec();
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
                         QMessageBox msgbox(QMessageBox::Warning,"提示","   卡中电量尚未入电表，不能购电！           ",0,this);
                         msgbox.addButton("确认", QMessageBox::YesRole);
                         msgbox.exec();
                         return -1;
                     }

                 }
             }
            /* else if(socket->Lk_msg.CardState=="02"||socket->Lk_msg.CardState=="04")
             {
                 if(socket->Lk_msg.ls_dlzt=="0")
                 {
                     QMessageBox msgbox(QMessageBox::Warning,"提示","   卡中电量尚未入电表，不能继续购电！          ",0,this);
                     msgbox.addButton("确认", QMessageBox::YesRole);
                     msgbox.exec();
                     return -1;
                 }
             }*/
          }
         else{
             QMessageBox msgbox(QMessageBox::Warning,"提示","   请使用开户卡，购电卡进行购电！           ",0,this);
             msgbox.addButton("确认", QMessageBox::YesRole);
             msgbox.exec();
             return -1;}
     }
    return 0;
}
/*费控卡参数验证*/
int Writecard::checkCard(QString &data1, QString &data2,QString &data3,QString &data4)
{

    QStringList list1=data1.split("/");
    QStringList list2=data2.split("/");
    QStringList list3=data3.split("/");
    socket->FK_msg.cardtype=list1.at(0);
    socket->FK_msg.gdcs1=list2.at(10);
    if(socket->FK_msg.cardtype=="00")
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"该卡是新卡，请开卡后进行处理！");
        return -1;
    }
    else if(socket->FK_msg.cardtype=="01"||socket->FK_msg.cardtype=="02"||socket->FK_msg.cardtype=="03")
    {
        socket->FK_msg.khbh=list1.at(10);
        socket->FK_msg.dbbh=list1.at(32);
        socket->FK_msg.kczcs=list3.at(1);
        socket->FK_msg.yhdabh=list1.at(10);
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
    CardType=0;
    socket->CardType=0;
    return 0;
}
void Writecard::replyFinished(QNetworkReply *reply)
{
    times++;
    if(reply->error() == QNetworkReply::NoError)
     {
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
          OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
          m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
        if(m_HttpData.at(0)=="ok")
        {
          //qDebug()<<"m_HttpData.size:"<<m_HttpData.size();
        /*接收到费控补卡状态信息*/
         if(m_HttpData.size()==7)
        {
                //times==2
                    messagebox->closeDialog();
                    socket->fkbkzt_msg.zhbl=m_HttpData.at(1);
                    socket->fkbkzt_msg.djmc=m_HttpData.at(2);
                    socket->fkbkzt_msg.dbxb=m_HttpData.at(3);
                    socket->fkbkzt_msg.djdm=m_HttpData.at(4);
                    socket->fkbkzt_msg.yffid=m_HttpData.at(5);
                    socket->fkbkzt_msg.result=m_HttpData.at(6);
                    if(socket->fkbkzt_msg.result=="0")
                    {
                        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                        messagebox->displayWithoutButton(this,"未查询到补卡信息");
                        return;
                    }
                    else
                    {
                        /*QString fk_writecardstr;
                        OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD",fk_writecardstr);
                        disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                        connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                        //send费控写卡接口
                        socket->HttpSend(QUrl(QString(fk_writecardstr).arg(socket->Card_msg.yhdabh).arg(socket->Card_msg.dbbh).arg(socket->LoginMessage.rybh).arg("02").arg(socket->FK_msg.fileReply).arg("").arg(socket->FK_msg.CurrentCardType)));
                        qDebug()<<QString(fk_writecardstr).arg(socket->Card_msg.yhdabh).arg(socket->Card_msg.dbbh).arg(socket->LoginMessage.rybh).arg("02").arg(socket->FK_msg.fileReply).arg("").arg(socket->FK_msg.CurrentCardType);*/
                        disconnectSlots();
                        times=0;
                        Writecardlist *form_writecardlist=new Writecardlist;
                        form_writecardlist->setAttribute(Qt::WA_DeleteOnClose);
                        socket->effect->begin(this, form_writecardlist,LEFTTORIGHT,NONE,HIDE);
                    }      

        }
         /*接收到量控补卡状态信息*/
         else if(m_HttpData.size()==3)
         {
               //times=2
              if(CardType==1)
              {
                  socket->lkbkzt_msg.num=m_HttpData.at(1);
                  if(m_HttpData.at(1)=="*")
                  {
                      QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                      messagebox->displayWithoutButton(this,"未查询到补卡信息");
                      return;
                  }
                  else
                  {
                      disconnectSlots();
                      times=0;
                      Writecardlist *form_writecardlist=new Writecardlist;
                      form_writecardlist->setAttribute(Qt::WA_DeleteOnClose);
                      socket->effect->begin(this, form_writecardlist,LEFTTORIGHT,NONE,HIDE);
                  }
              }
         }
        /*接收到补卡用户信息*/
        else if(m_HttpData.size()==8)
        {
                //times==1
                socket->writecard_msg.yyzbh=m_HttpData.at(1);
                socket->writecard_msg.yhmc=m_HttpData.at(2);
                socket->writecard_msg.yhdabh=m_HttpData.at(3);
                socket->writecard_msg.sfznum=m_HttpData.at(4);
                socket->writecard_msg.address=m_HttpData.at(5);
                socket->writecard_msg.phone=m_HttpData.at(6);
                socket->writecard_msg.ydhymc=m_HttpData.at(7);
                if(CardType==0)
                {

                    QString fk_str;
                    OperateFile::readiniFile("INTERFACE.DATA","interface/GETFKBKZT",fk_str);
                    socket->HttpSend(QUrl(QString(fk_str).arg(socket->writecard_msg.yhdabh).arg(socket->FK_msg.dbbh)));
                   // socket->HttpSend(QUrl("http://122.113.5.232:8040/zzjfnew/get_fkbkzt.jsp?xmlData=<DATA><YHDABH>020100001487</YHDABH><JLBBH>000000000008</JLBBH></DATA>"));
                }
                else if(CardType==1)
                {
                    QString lk_str;
                    OperateFile::readiniFile("INTERFACE.DATA","interface/GETLKBKZT",lk_str);
                    socket->HttpSend(QUrl(QString(lk_str).arg(socket->writecard_msg.yhdabh).arg(socket->LK_msg.jlbbh)));
                    qDebug()<<QString((lk_str).arg(socket->writecard_msg.yhdabh).arg(socket->LK_msg.jlbbh));
                }

        }
        }
        else if(m_HttpData.at(0)=="error")
        {

            messagebox->closeDialog();
            if(times==1)
            {
               //用户补卡信息
               OperateFile::tracelog("获取补卡信息"+QString::fromLocal8Bit(m_receivedata));
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"未查询到用户信息！");
               times=0;
            }
            else if(times>=2)
            {
                //补卡状态信息
               OperateFile::tracelog("获取补卡状态信息"+QString::fromLocal8Bit(m_receivedata));
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"未查询到补卡信息！");
               times=0;
            }
            ui->pushButton_readcard->setEnabled(true);
        }
    }
    else
    {
          ui->pushButton_readcard->setEnabled(true);
          messagebox->closeDialog();
          OperateFile::tracelog("server refuse");
          QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
          messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
    }
    reply->deleteLater();
}
/*void Writecard::replyFinished_getList(QNetworkReply *reply)
{
    qDebug()<<"getlist_httprecive";
    if(reply->error() == QNetworkReply::NoError)
     {
        char m_receivedata[2048];
        memset(m_receivedata,0,2048);
        reply->read(m_receivedata,2048);
        qDebug()<<QString::fromLocal8Bit(m_receivedata);
        m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
        if(m_HttpData.at(0)=="ok")
        {
            Writecardlist *form_writecardlist=new Writecardlist;
            form_writecardlist->setAttribute(Qt::WA_DeleteOnClose);
            socket->effect->begin(this, form_writecardlist,LEFTTORIGHT,NONE,HIDE);
        }
        else if(m_HttpData.at(0)=="error")
        {
            messagebox->dis

        }
    }
    else
    {
          OperateFile::tracelog("server refuse");
          qDebug()<<"server refuse";//服务器拒绝访问
    }
    reply->deleteLater();
}*/
/*void Writecard::replyFinished_writecard(QNetworkReply *reply)
{
    write_times++;
    qDebug()<<"http_carddatarecive";
    if(reply->error() == QNetworkReply::NoError)
     {
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
          //对返回串做处理
           //写日志*
          qDebug()<<QString::fromLocal8Bit(m_receivedata);
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
          qDebug()<<"m_HttpData.size:"<<m_HttpData.size();
         //收到写卡查询信息信息
         if(m_HttpData.at(0)=="ok")
         {

            if(m_HttpData.size()==4)
             {
                 //费控数据查询信息
                 socket->FK_datacheck.bcgdje=m_HttpData.at(1);
                 socket->FK_datacheck.bcgdye=m_HttpData.at(2);
                 socket->FK_datacheck.fpqybz=m_HttpData.at(3);
                 socket->canwritecard=true;
                 Writecardlist *form_writecardlist=new Writecardlist;
                 form_writecardlist->unwrite_cardmsg=socket->FK_msg;
                 form_writecardlist->unwrite_msg=socket->FK_writewithcard;
                 qDebug()<<"11czje:"<<form_writecardlist->unwrite_msg.czje;
                 qDebug()<<"11filemony"<< form_writecardlist->unwrite_msg.fileMoney;


                 disconnectSlots();
                 form_writecardlist->setAttribute(Qt::WA_DeleteOnClose);
                 socket->effect->begin(this, form_writecardlist,LEFTTORIGHT,NONE,HIDE);
             }
             //收到费控写卡信息
             else if(m_HttpData.size()==12)
             {
                if(m_HttpData.at(1)=="0")
                {
                     socket->flag=0;
                }
                 //times=1
                else
                {
                 socket->flag=1;
                 socket->FK_writewithcard.jfje=m_HttpData.at(2);
                 socket->FK_writewithcard.czje=m_HttpData.at(3);
                 socket->FK_writewithcard.czcs=m_HttpData.at(4);
                 socket->FK_writewithcard.guiyue=m_HttpData.at(5);
                 socket->FK_writewithcard.writetype=m_HttpData.at(6);
                 socket->FK_writewithcard.byteof8=m_HttpData.at(7);
                 socket->FK_writewithcard.fileParam=m_HttpData.at(8);
                 socket->FK_writewithcard.fileMoney=m_HttpData.at(9);
                 qDebug()<<"czje:"<<socket->FK_writewithcard.czje;
                 qDebug()<<"filemony"<< socket->FK_writewithcard.fileMoney;
                 socket->FK_writewithcard.filePrice1=m_HttpData.at(10);
                 socket->FK_writewithcard.filePrice2=m_HttpData.at(11);
                }
                 //send 费控写卡数据查询
                 QString string;
                 OperateFile::readiniFile("INTERFACE.DATA","interface/FKDATASEARCH",string);

                 socket->HttpSend(QUrl(QString(string).arg(socket->Card_msg.yhdabh).arg(socket->LoginMessage.gsbh)));
                 qDebug()<<"httpsendFKdatasearch";
                 qDebug()<<QString(string).arg(socket->Card_msg.yhdabh).arg(socket->LoginMessage.gsbh);

         }
         }
             else if(m_HttpData.at(0)=="error")
             {
                  //写卡数据获取失败
                if(write_times==1)
                {
                    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                    QMessageBox msgbox(QMessageBox::Warning,"未识别的卡","       卡数据信息获取失败！           ",0,this);
                    msgbox.addButton("确认", QMessageBox::YesRole);
                    msgbox.exec();
                    OperateFile::tracelog("写卡数据获取失败"+QString::fromLocal8Bit(m_receivedata));
                    return ;
                }
                //卡数据信息查询失败
                else if(write_times==2)
                {

                    write_times=0;
                    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                    QMessageBox msgbox(QMessageBox::Warning,"未识别的卡","       卡数据信息查询失败！           ",0,this);
                    msgbox.addButton("确认", QMessageBox::YesRole);
                    msgbox.exec();
                   OperateFile::tracelog("写卡数据信息查询失败"+QString::fromLocal8Bit(m_receivedata));
                    return ;
                }
             }
     }
    else
    {
          qDebug()<<"server refuseeee";//服务器拒绝访问
    }
    reply->deleteLater();
}*/

void Writecard::on_pushButton_readcard_clicked()
{
    startTimer();
    Timeoutflag=true;
    messagebox->displayWithoutBlock(this,"正在读卡");
    socket->sendMsg("Network&9019&1&EC_ReadCard");
}
void Writecard::on_pushButton_back_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
}
void Writecard::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_10->show();
}
void Writecard::startTimer()
{
    emit start(MSEC);
}
void Writecard::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_10->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
void Writecard::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       messagebox->closeDialog();
       if(Timeoutflag==true)
       {
           OperateFile::tracelog("in writecard:quest timeout");
           QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"请求超时，请到柜台办理业务");

       }
       disconnectSlots();
       socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
    }
    else
    {

        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
//底部- 缴费购电按钮
void Writecard::on_pushButton_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_purchase,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_purchase->init();
}
//底部-信息查询
void Writecard::on_pushButton_2_clicked()
{
    disconnectSlots();
    socket->IsPurchase=false;
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this, OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_readcard->init();
}
//公共服务
void Writecard::on_pushButton_3_clicked()
{

}

void Writecard::on_pushButton_backHomePage_2_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
}
void Writecard::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(writecard_GetData()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)));
}

void Writecard::on_pushButton_back_2_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_purchase,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_purchase->init();
}
void Writecard::connectError(QAbstractSocket::SocketError socketError)
{
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"UMP未启动！");
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
    }
}
