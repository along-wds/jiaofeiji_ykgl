/*************2017-05-04****************/
#include "msgconfirm.h"
#include "ui_msgconfirm.h"
#include"cashpay.h"
#include"unionpay.h"
#include"readcard.h"
#include"inputamount.h"
#include"writecardlist.h"
#include"homepage.h"
#include"writecardfinished.h"
#include "lookup.h"
extern commonDialog *messagebox;
MsgConfirm::MsgConfirm(QWidget *parent) :
    CommonWidget(0,0,parent),
   ui(new Ui::MsgConfirm)
{
   ui->setupUi(this);
   ui->frame->setObjectName("frame");
   ui->frame_2->setObjectName("frame2");
   ui->frame_4->setObjectName("frame4");
   ui->frame_3->setObjectName("frame3");
   ui->frame_5->setObjectName("frame5");
   ui->groupBox->setObjectName("groupbox1");
   ui->groupBox->setStyleSheet("QGroupBox#groupbox1{border-bottom-color: darkgray;border-bottom-width: 1px;}");
   ui->frame_4->setStyleSheet("QFrame#frame4{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
   ui->frame_5->setStyleSheet("QFrame#frame5{background-color:#dfdfdf;border:1px;border-radius:3px;}");
   //ui->frame_3->setStyleSheet("QFrame#frame3{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
   ui->pushButton_backHomePage_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
   ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                 "QPushButton:disabled{border-image: url(:/image/picture/qietu/purchase+.png);}");
   ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                   "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
   ui->pushButton_4->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                   "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
   //ui->frame->setStyleSheet("QFrame#frame{border:2px groove gray;border-radius:10px;background-color:white;}");
   //ui->frame_2->setStyleSheet("QFrame#frame2{border:2px groove gray;border-radius:10px;background-color:white;}");
   //ui->label_6->setStyleSheet("QLabel{color: white}");
   ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
   //调色板
   QPalette lcdpat=ui->lcdNumber->palette();
   /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
   lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
   //设置当前窗口的调色板
   ui->lcdNumber->setPalette(lcdpat);
}
void MsgConfirm::init()
{
   ui->pushButton_ConfirmPurchase->setEnabled(true);
   ui->pushButton_backHomePage_2->setEnabled(true);
   ui->pushButton->setEnabled(false);
   ui->pushButton_3->setEnabled(true);
   ui->pushButton_4->setEnabled(true);
   ui->pushButton_2->setEnabled(true);
   ui->pushButton_back->setEnabled(true);

   connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
   connect(socket,SIGNAL(readyRead()),this,SLOT(msgconfirm_GetData()),Qt::UniqueConnection);
   connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
   connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
   connect(this, SIGNAL(stop()),ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
   connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
   connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
   connect(choicebox,SIGNAL(cancel_clicked()),this,SLOT(closeChoiceDialog()),Qt::QueuedConnection);
   connect(choicebox,SIGNAL(ok_clicked()),this,SLOT(jumpToWriteList()),Qt::QueuedConnection);
   display();
   startTimer();
}
void MsgConfirm::replyFinished(QNetworkReply *reply)
{
    socket->is_wait_http_resp=false;
    if(reply->error() == QNetworkReply::NoError)
     {
          QString string;
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
          OperateFile::tracelog("in Msgconfirm Http received:\t"+QString::fromLocal8Bit(m_receivedata));
          m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
          QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
          if(base_arg.at(0)=="ok")
          {
              if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
              {
                  OperateFile::tracelog("error argument");
                  return;
              }
             if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETFKBKZT)
             {

                     messagebox->closeDialog();
                     socket->fkbkzt_msg.zhbl=m_HttpData.at(1);
                     socket->fkbkzt_msg.djmc=m_HttpData.at(2);
                     socket->fkbkzt_msg.dbxb=m_HttpData.at(3);
                     socket->fkbkzt_msg.djdm=m_HttpData.at(4);
                     socket->fkbkzt_msg.yffid=m_HttpData.at(5);
                     socket->fkbkzt_msg.result=m_HttpData.at(6);
                     if(socket->fkbkzt_msg.result=="0")
                     {

                        messagebox->closeDialog();
                        choicebox->display(this,"您没有未充值的缴费信息，是否进行补卡操作？");

                     }
                     else
                     {
                         messagebox->displayWithoutBlock(this,"正在获取写卡信息，请稍后……");
                         QString fk_writecardstr;
                         OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD_ZX",fk_writecardstr);
                         if(socket->CardType==0)
                         {
                            socket->HttpSend(QUrl(QString(fk_writecardstr).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("0").arg("").arg("")));
                            OperateFile::tracelog(QString(fk_writecardstr).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("0").arg("").arg(""));
                         }
                         else if(socket->CardType==3)
                         {
                             socket->HttpSend(QUrl(QString(fk_writecardstr).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh)));
                             OperateFile::tracelog(QString(fk_writecardstr).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh));

                         }
                      }
             }
             else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETLKBKZT)
             {
                    messagebox->closeDialog();
                    socket->lkbkzt_msg.num=m_HttpData.at(1);
                    socket->lkbkzt_msg.je=m_HttpData.at(2);
                    if(socket->lkbkzt_msg.num!="*")
                    {
                        messagebox->closeDialog();
                        choicebox->display(this,"您没有未充值的缴费信息，是否进行补卡操作？");
                    }
                    else
                    {
                            QString lk_writecardstr;
                            OperateFile::readiniFile("INTERFACE.DATA","interface/LKWRITECARD_ZX",lk_writecardstr);
                            socket->HttpSend(QUrl(QString(lk_writecardstr).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg(socket->lkbkzt_msg.je).arg("0").arg("").arg(socket->Card_Basemsg.FileDate1)));
                            OperateFile::tracelog(QString(lk_writecardstr).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg(socket->lkbkzt_msg.je).arg("0").arg("").arg(socket->Card_Basemsg.FileDate1));
                    }

             }
             else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::FKWRITECARD)
             {
                 socket->FK_writewithcard.jfje=m_HttpData.at(1);
                 socket->FK_writewithcard.czje=m_HttpData.at(2);
                 socket->FK_writewithcard.czcs=m_HttpData.at(3);
                 socket->FK_writewithcard.ccs1=m_HttpData.at(4);
                 socket->FK_writewithcard.ccs2=m_HttpData.at(5);
                 socket->FK_writewithcard.ccs3=m_HttpData.at(6);
                 socket->FK_writewithcard.szRandFromHsm=m_HttpData.at(7);
                 socket->FK_writewithcard.szMac1=m_HttpData.at(8);
                 socket->FK_writewithcard.szMac2=m_HttpData.at(9);
                 socket->FK_writewithcard.szMac3=m_HttpData.at(10);
                 socket->FK_writewithcard.szMac4=m_HttpData.at(11);
                 socket->FK_writewithcard.szDes1=m_HttpData.at(12);
                 socket->FK_writewithcard.szDes2=m_HttpData.at(13);
                 socket->FK_writewithcard.szKeyData=m_HttpData.at(14);
                 //send 费控写卡数据查询
                 OperateFile::readiniFile("INTERFACE.DATA","interface/FKDATASEARCH",string);

                 socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
                 OperateFile::tracelog(QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));
             }
             else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LKWRITECARD)
             {
                 socket->LK_writewithcard.gdl=m_HttpData.at(1);
                 socket->LK_writewithcard.xkdl=m_HttpData.at(2);
                 socket->LK_writewithcard.gdcs=m_HttpData.at(3);
                 socket->LK_writewithcard.ccs1=m_HttpData.at(4);
                 //send 量控写卡数据查询
                 OperateFile::readiniFile("INTERFACE.DATA","interface/LKDATASEARCH",string);
                 socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
             }
             else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::FKDATASEARCH)
             {
                 socket->FK_datacheck.bcgdje=m_HttpData.at(1);
                 socket->FK_datacheck.bcgdye=m_HttpData.at(2);
                 socket->FK_datacheck.fpqybz=m_HttpData.at(3);
                 writecard();
             }
             else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LKDATASEARCH)
             {
                 socket->LK_datacheck.bcgdje=m_HttpData.at(1);
                 socket->LK_datacheck.bcgdl=m_HttpData.at(2);
                 socket->LK_datacheck.bcgdye=m_HttpData.at(3);
                 socket->LK_datacheck.fpqybz=m_HttpData.at(4);
                 writecard();
             }
        }
        else if(base_arg.at(0)=="error")
        {

            messagebox->closeDialog();
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETLKBKZT)
            {
              choicebox->display(this,"您没有未充值的缴费信息，是否进行补卡操作？");
            }
            else
            {
               //补卡状态信息
               OperateFile::tracelog("获取补卡状态信息"+QString::fromLocal8Bit(m_receivedata));
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"未查询到补卡信息！");
            }
        }
    }
    else
    {
          messagebox->closeDialog();
          OperateFile::tracelog("server refuse");
          QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
          messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
    }
    reply->deleteLater();
}
MsgConfirm::~MsgConfirm()
{
   delete ui;
  // qDebug()<<"delete msgconfirm";
}

void MsgConfirm::msgconfirm_reshow()
{
   //init();
}
void MsgConfirm::showTzje()
{
   ui->label_AdjustNum->show();
   ui->label_13->show();
   ui->label_18->show();
}
void MsgConfirm::hideTzje()
{
   ui->label_AdjustNum->hide();
   ui->label_13->hide();
   ui->label_18->hide();
}
void MsgConfirm::display()
{
   int namesize=socket->message.yhmc.size();
   int phonesize=socket->message.phone.size();
   int sfzhsize=socket->message.sfznum.size();
   QString Displayname=socket->message.yhmc;
   QString Displayphone=socket->message.phone;
   QString Displaysfzh=socket->message.sfznum;
   if(namesize!=0)
   {
       Displayname=Displayname.replace(0,1,"*");
   }
   if(phonesize!=0&&phonesize>7)
   {
       Displayphone=Displayphone.replace(3,4,"****");
   }
   if(sfzhsize!=0&&sfzhsize>7)
    {
        Displaysfzh=socket->message.sfznum.left(3)+socket->message.sfznum.right(4);
        for(int i=0;i<sfzhsize-7;i++)
        {
           Displaysfzh.insert(3+i,"*");
        }
    }
   ui->label_ClientName->setText(socket->message.yhmc);
   ui->label_ClientNum->setText(socket->message.yhdabh);
   ui->label_ContactTelephone->setText(Displayphone);
   ui->label_address->setText(socket->message.Address);
   ui->label_AdjustNum->setText(socket->message.yzje);//调整金额
   ui->label_AccountBalance->setText(socket->message.zhye);//账户余额
   ui->label_PenaltyNum->setText(socket->message.wyj);     //违约金
   ui->label_PaymentAccount->setText(socket->message.xzje);//应交金额应大于
   ui->label_PowerTrade->setText(socket->message.ydhymc);
   ui->label_ClientName_2->setText(Displaysfzh);
   QPalette pe;
   pe.setColor(QPalette::WindowText,"#ffaa00");
   ui->label_AdjustNum->setPalette(pe);//调整金额
   ui->label_AccountBalance->setPalette(pe);//账户余额
   ui->label_PenaltyNum->setPalette(pe);     //违约金
   ui->label_PaymentAccount->setPalette(pe);//应交金额应大于
}
//现金交费
void MsgConfirm::on_pushButton_ConfirmPurchase_clicked()
{
#ifdef DEBUG
    disconnectSlots();
    socket->Unionpaychannel=true;
    socket->PurchaseType=0;
    OperateFile::ui_inputamount=new inputamount;
    OperateFile::ui_inputamount->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this, OperateFile::ui_inputamount,LEFTTORIGHT,NONE,HIDE);
    //OperateFile::ui_inputamount->init();
#else
    if(OperateFile::ui_homepage->Couldpurchase==false)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,OperateFile::ui_homepage->warnMsg);
        return;
    }
    if(socket->CardType==2&&socket->message.count.toInt()>1&&socket->message.jxjf=="no")
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"此户为非普通多表用户，请到营业窗口缴费！");
        return;
    }
   if(socket->message.fffs=="02"&&socket->message.jeqybz=="1"&&((socket->message.ickye> socket->message.ickxzje)||(socket->message.zhye< socket->message.zmxzje)))
    {

        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您的账户余额存在异常，请到电力营业厅柜台处理!");
        return;
    }
    disconnectSlots();
    socket->Unionpaychannel=true;
    socket->PurchaseType=0;
    ui->pushButton_ConfirmPurchase->setEnabled(false);
    OperateFile::ui_cashpay=new CashPay();
    OperateFile::ui_cashpay->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this, OperateFile::ui_cashpay,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_cashpay->init();

#endif
}
void MsgConfirm::on_pushButton_back_clicked()
{
   ui->pushButton_back->setEnabled(false);
   disconnectSlots();
   socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
   //OperateFile::ui_homepage->init();
}

void MsgConfirm::on_pushButton_2_clicked()
{
    messagebox->displayWithoutBlock(this,"正在获取补卡信息……");
    socket->sendMsg("Network&9019&1&EC_GetState");

}
//底部-首页
void MsgConfirm::on_pushButton_backHomePage_2_clicked()
{
   ui->pushButton_backHomePage_2->setEnabled(false);
   disconnectSlots();
   socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
   //OperateFile::ui_homepage->init();
}
//底部- 缴费购电按钮
void MsgConfirm::on_pushButton_clicked()
{
   //ui->pushButton->setEnabled(false);
  // disconnectSlots();
   //socket->effect->begin(this, OperateFile::ui_purchase,RIGHTTOLEFT,NONE,HIDE);
}
void MsgConfirm::waitTimeout()
{
   emit updateDispaly();
   ui->lcdNumber->show();
   ui->label->show();
}
//底部-信息查询
void MsgConfirm::on_pushButton_3_clicked()
{
   ui->pushButton_3->setEnabled(false);
   disconnectSlots();

   if(OperateFile::ui_lookup==0)
   {
       OperateFile::ui_lookup=new Lookup();
   }
   socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
   //OperateFile::ui_lookup->init();

}
//公共服务
void MsgConfirm::on_pushButton_4_clicked()
{

}
void MsgConfirm::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->label->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void MsgConfirm::startTimer()
{
   m_currenttime=SEC;
   ui->lcdNumber->display(SEC);
   ui->label->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void MsgConfirm::setLcdnum()
{
   if(ui->lcdNumber->value()==0)
   {

       disconnectSlots();
       socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
       //OperateFile::ui_homepage->init();
   }
   else
   {
       m_currenttime--;
       ui->lcdNumber->display(m_currenttime);
   }
}
void MsgConfirm::disconnectSlots()
{
   emit stop();
   disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
   disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
   disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
   disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
   disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
   disconnect(socket,SIGNAL(readyRead()),this,SLOT(msgconfirm_GetData()));
   disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
   disconnect(choicebox,SIGNAL(cancel_clicked()),this,SLOT(closeChoiceDialog()));
   disconnect(choicebox,SIGNAL(ok_clicked()),this,SLOT(jumpToWriteList()));
   socket->Unionpaychannel=true;
}


void MsgConfirm::on_pushButton_ylPurchase_clicked()
{
    /*银联缴费*/
    if(OperateFile::ui_homepage->Couldpurchase==false)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,OperateFile::ui_homepage->warnMsg);
        return;
    }
    if(socket->CardType==2&&socket->message.count.toInt()>1&&socket->message.jxjf=="no")
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"此户为非普通多表用户，请到营业窗口缴费！");
        return;
    }
   if(socket->message.fffs=="02"&&socket->message.jeqybz=="1"&&((socket->message.ickye> socket->message.ickxzje)||(socket->message.zhye< socket->message.zmxzje)))
    {

        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您的账户余额存在异常，请到电力营业厅柜台处理!");
        return;
    }
   disconnectSlots();   //断开槽
   socket->Unionpaychannel=true;
   socket->PurchaseType=1;
   if(OperateFile::ui_unionpay==0)
    {
       OperateFile::ui_unionpay=new UnionPay;
       connect(OperateFile::ui_unionpay,SIGNAL(back_MsgConfirm()),this,SLOT(msgconfirm_reshow()),Qt::UniqueConnection);
    }
   socket->effect->begin(this, OperateFile::ui_unionpay,LEFTTORIGHT,NONE,HIDE);
   //OperateFile::ui_unionpay->init();
   OperateFile::ui_unionpay->registtoUnion();

}
void MsgConfirm::jumpToWriteList()
{
    disconnectSlots();
    Writecardlist *form_writecardlist=new Writecardlist;
    form_writecardlist->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this, form_writecardlist,LEFTTORIGHT,NONE,HIDE);
}
void MsgConfirm::closeChoiceDialog()
{
    qDebug()<<"do nothing";
}
void MsgConfirm::writecard()
{
    //费控
    messagebox->closeDialog();
    messagebox->displayWithoutBlock(this,"正在写卡，请勿拔出电卡");
    if(socket->CardType==0)
    {

        socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4").arg(socket->CardType).arg(socket->FK_writewithcard.ccs1).arg(socket->FK_writewithcard.ccs2).arg(socket->FK_writewithcard.ccs3));

    }
    //量控
    else if(socket->CardType==1)
    {

        socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2").arg(socket->CardType).arg(socket->LK_writewithcard.ccs1));

    }
    else if(socket->CardType==3)
    {

        socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4_%5_%6_%7_%8_%9_%10_%11_%12").arg(socket->CardType).arg(socket->FK_writewithcard.ccs1).arg(socket->FK_writewithcard.ccs2)\
                        .arg(socket->FK_writewithcard.ccs3).arg(socket->FK_writewithcard.szRandFromHsm).arg(socket->FK_writewithcard.szMac1).arg(socket->FK_writewithcard.szMac2).arg(socket->FK_writewithcard.szMac3)\
                        .arg(socket->FK_writewithcard.szMac4).arg(socket->FK_writewithcard.szDes1).arg(socket->FK_writewithcard.szDes2).arg(socket->FK_writewithcard.szKeyData));

    }

}

void MsgConfirm::msgconfirm_GetData()
{
    OperateFile::tracelog("TCP receive:"+socket->ToString());
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }
    if(list.at(2)=="GetState")
    {
        if(list.at(3)!="0")
        {
           // 获取卡类型失败
            messagebox->closeDialog();//关闭最初对话框
            disconnectSlots();
            QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"您已将原电卡拔出，无法写卡，请到柜台处理");
            return;
        }
        else
        {
            socket->CardType=QString(list.at(4)).toInt();
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(socket->CardType)));
        }
        return;

    }
    if(list.at(2)=="ReadCard")
    {
        if(socket->CardType==0||socket->CardType==3)
        {
            if(socket->CardType==3)
            {
                socket->Card_Basemsg.szCardNum=list.at(5);
                socket->Card_Basemsg.szRandFromCard=list.at(6);
            }
            QString fk_str;
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETFKBKZT",fk_str);
            socket->HttpSend(QUrl(QString(fk_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh)));
            OperateFile::tracelog(QString(fk_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh));
        }
        else if(socket->CardType==1)
        {
            QString lk_str;
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETLKBKZT",lk_str);
            socket->HttpSend(QUrl(QString(lk_str).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.yyzbh).arg(socket->LK_msg.ickid)));
            OperateFile::tracelog(QString(lk_str).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.yyzbh).arg(socket->LK_msg.ickid));
        }
    }
    else if(list.at(2)=="WriteCard")
    {
        disconnectSlots();
        messagebox->closeDialog();
        if(list.at(4)!="0")
        {

            QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡失败，请到柜台处理");
            return;
        }
        WritecardFinished *form_writecardfinish=new WritecardFinished();
        form_writecardfinish->setAttribute(Qt::WA_DeleteOnClose);
        //form_writecardfinish->init();
        socket->effect->begin(this, form_writecardfinish,LEFTTORIGHT,NONE,HIDE);

   }

}
