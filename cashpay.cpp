#include "cashpay.h"
#include "ui_cashpay.h"
#include"inputpassword.h"
#include"first.h"
#include"readcard.h"
extern commonDialog *messagebox;
CashPay::CashPay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CashPay)
{
    ui->setupUi(this);
    form_dealfinish=new dealfinish();
    form_dealfinish->setAttribute(Qt::WA_DeleteOnClose);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(check_CurrenMoney()));
    totalsum=0;
    BILLCHECKSTATE=NOMCHECK;
    IsAccaptResponse=false;
    bill_error_times=0;
    ui->frame_5->setObjectName("frame5");
    ui->frame_6->setObjectName("frame6");
    ui->frame->setObjectName("frame1");
    ui->frame_2->setObjectName("frame2");
    ui->label_2->setStyleSheet("color:#ffaa00;font-size:25px;font-family: \"Microsoft YaHei UI\";");
    ui->label_4->setStyleSheet("color:#ffaa00;font-size:25px;font-family: \"Microsoft YaHei UI\";");
    ui->frame->setStyleSheet("QFrame#frame1{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->frame_6->setStyleSheet("QFrame#frame6{background-color:#dfdfdf;border:1px;border-radius:3px;}");
    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}

CashPay::~CashPay()
{
    delete timer;
    delete ui;
}
void CashPay::init()
{
    ui->pushButton_purchase->setEnabled(false);
    connect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_form_bill()),Qt::UniqueConnection);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(connectError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    startTimer();
    messagebox->displayWithoutBlock(this,"初始化中……");
    socket->sendMsg("Pipe&PipeBill&1&billstate");
}
void CashPay::cashPay_GetData_form_bill()
{
    OperateFile::tracelog("TCP receive:"+socket->ToString());
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }
    if(list.at(1)=="billstate")
    {
        BILLSTATE=BEGIN;
        if(list.at(2)=="ok")
        {
            socket->sendMsg("Pipe&PipeBill&1&begin");
        }
        else
        {
            bill_error_times++;
            if(bill_error_times<=1)
            {
                socket->sendMsg("Pipe&PipeBill&1&reset");
            }
            else
            {
                bill_error_times=0;
                socket->sendMsg("Pipe&PipeBill&1&begin");
            }
        }
    }
    else if(list.at(1)=="reset")
    {
       socket->sendMsg("Pipe&PipeBill&1&begin");
    }
    //开始放钞请求
    else if(list.at(1)=="begin")
    {
        if(list.at(2)=="ok")
        {
            messagebox->closeDialog();
            IsAccaptResponse=true;
            socket->sendMsg("Pipe&PipeBill&1&accept");
        }
        else
        {
            bill_error_times++;
            if(bill_error_times<=1)
            {
                socket->sendMsg("Pipe&PipeBill&1&begin");
            }
            else
            {
                bill_error_times=0;
                messagebox->closeDialog();
                OperateFile::tracelog("cashpay begin return ret:"+QString(list.at(3)));
                disconnectSlots();
                QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"现金缴费服务暂不可用，请稍后办理");
                socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
                OperateFile::ui_homepage->init();
            }
        }
    }
    else if(list.at(1)=="accept")
    {
        IsAccaptResponse=false;
        startTimer();
        if(list.at(2)=="ok")
        {
            currentsum=QString(list.at(3)).toInt();
            totalsum+=currentsum;
            ui->label_2->setText(QString::number(currentsum));
            ui->label_4->setText(QString::number(totalsum));
            startTimer();
            /************did not test***********/
            if(BILLCHECKSTATE==SECCHECK)
            {
                BILLCHECKSTATE=LASCHECK;
                socket->sendMsg("Pipe&PipeBill&1&accapt");
            }
            else if(BILLCHECKSTATE==NOMCHECK) timer->start(50);
            else if(BILLCHECKSTATE==LASCHECK) socket->sendMsg("Pipe&PipeBill&1&end");

        }
        else
        {
            if(BILLCHECKSTATE==SECCHECK)
            {
                BILLCHECKSTATE=LASCHECK;
                OperateFile::tracelog("===========send the last check============");
                timer->start(50);
            }
            else if(BILLCHECKSTATE==NOMCHECK) timer->start(50);
            else if(BILLCHECKSTATE==LASCHECK) socket->sendMsg("Pipe&PipeBill&1&end");
        }
    }
    else if(list.at(1)=="end")
    {

        if(list.at(2)!="ok")
        {
            OperateFile::tracelog("bill end of error:"+socket->ToString());
        }
            BILLSTATE=END;
            if(totalsum!=0)
            {
                //开始进行交易
                QString inistring;
                GetSerialNumber();
                OperateFile::readiniFile("INTERFACE.DATA","interface/CASHPAY",inistring);
                socket->HttpSend(QUrl(QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                .arg(QString::number(totalsum)).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                                .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh)));

                OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg("").arg(socket->message.ydhy).arg("null").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                      .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                      .arg(QString::number(totalsum)).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("null").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("null")
                                      .arg(socket->message.phone).arg("null").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh));


            /*
              socket->HttpSend(QUrl(QString(inistring).arg("").arg(socket->message.ydhy).arg("null").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
              .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
              .arg(QString::number(totalsum)).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("null").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->IP).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("null")
              .arg(socket->message.phone).arg("null").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh)));

             */
            }
            else if(totalsum==0)
            {
                messagebox->closeDialog();
                disconnectSlots();
                socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
                OperateFile::ui_homepage->init();
            }

    }

}
void CashPay::cashPay_GetData_from_EC()
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
            socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
            form_dealfinish->init(3);
        }
        else
        {
            socket->CardType=QString(list.at(4)).toInt();
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(socket->CardType)));
        }

    }

    if(list.at(2)=="ReadCard")
    {
        if(socket->CardType==0)
        {
            QString fk_str;
            OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD_ZX",fk_str);
            socket->HttpSend(QUrl(QString(fk_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("0").arg("").arg("")));
            OperateFile::tracelog(QString(fk_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("0").arg("").arg(""));

        }
        else if(socket->CardType==1)
        {
            QString lk_str;
            OperateFile::readiniFile("INTERFACE.DATA","interface/LKWRITECARD_ZX",lk_str);
            socket->HttpSend(QUrl(QString(lk_str).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg(socket->amount_str).arg("0").arg("").arg(socket->Card_Basemsg.FileDate1)));
            OperateFile::tracelog("inputmount receive:"+ QString(lk_str).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg(socket->amount_str).arg("0").arg("").arg(socket->Card_Basemsg.FileDate1));
        }

        else if(socket->CardType==3)
        {
            QString zx_str;
            socket->Card_Basemsg.szCardNum=list.at(5);
            socket->Card_Basemsg.szRandFromCard=list.at(6);
            OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD_ZX",zx_str);
            socket->HttpSend(QUrl(QString(zx_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh)));
            OperateFile::tracelog(QString(zx_str).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("0").arg("").arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh));
        }

    }
    else if(list.at(2)=="WriteCard")
    {
        messagebox->closeDialog();
        if(list.at(4)!="0")
        {

            disconnectSlots();
            QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡失败，请到柜台处理");
            socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
            form_dealfinish->init(3);

        }
        else
        {
            disconnectSlots();
            socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
            form_dealfinish->init(0);
        }
   }
    else if(list.at(2)=="Purchase")
    {
        messagebox->closeDialog();
        if(list.at(3)!="0")
        {

            disconnectSlots();
            QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡失败，请到柜台处理");
            socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
            form_dealfinish->init(2);
        }
        else
        {
           disconnectSlots();
           socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
           form_dealfinish->init(0);
        }

    }
   else if(list.at(1)=="getstate")
   {
        if(list.at(2)=="ok"||list.at(3)=="1003")
        {
            QString PrintString;
            QString CurrentTime,currentdevid;
            OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",currentdevid);
            /*if(socket->OutData.strRespcode!="00")
                yl_result="失败";
            else
                yl_result="成功";*/
            CurrentTime=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
            PrintString.append("        内蒙古电力自助缴费终端消费凭条\r\n");
            PrintString.append("----------------------------------------------\r\n");
            PrintString.append("用户名称:"+socket->message.yhmc+"\r\n");
            PrintString.append("用户编号:"+socket->message.yhdabh+"\r\n");
            PrintString.append("缴费金额:"+ui->label_4->text()+" 元"+"\r\n");
            PrintString.append("  终端号:"+currentdevid+"\r\n");
            PrintString.append("  流水号:"+dllsh+"\r\n");
            PrintString.append("缴费方式:自助缴费终端_现金\r\n");
            PrintString.append("缴费结果:失败\r\n");
            PrintString.append("缴费时间:"+CurrentTime+"\r\n");
            PrintString.append("----------------------------------------------\r\n");
            PrintString.append(socket->message.fgsmc+"\r\n");
            PrintString.append(socket->message.gsmc+"\r\n");
            PrintString.append("\r\n\r\n\r\n\r\n\r\n");
            socket->sendMsg("Network&9008&1&Printer_Print_"+PrintString);
        }
        else
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            messagebox->display(this,"缴费失败，请到柜台完成缴费");
            backHomeBroken();
        }
    }
    else if(list.at(1)=="print")
    {
        if(list.at(2)!="ok")
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            messagebox->display(this,"缴费失败，请到柜台完成缴费");
        }
        else
        {
           messagebox->display(this,"缴费失败，请持凭条到柜台完成缴费");
        }
        backHomeBroken();
    }
}
void CashPay::replyFinished(QNetworkReply* reply)
{

    QString string;
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         reply->read(m_receivedata,4096);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::CASHPAY)
            {

                socket->YL_msg={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10)};
                socket->DataBase.insertData(socket->message.yhdabh,socket->message.yhmc,"",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),socket->YL_msg.sjje,"0","1","");
                readywriteCard();

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
                disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
                OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LKWRITECARD)
            {
                socket->LK_writewithcard.gdl=m_HttpData.at(1);
                socket->LK_writewithcard.xkdl=m_HttpData.at(2);
                socket->LK_writewithcard.gdcs=m_HttpData.at(3);
                socket->LK_writewithcard.ccs1=m_HttpData.at(4);
                //send 量控写卡数据查询
                OperateFile::readiniFile("INTERFACE.DATA","interface/LKDATASEARCH",string);
                disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                socket->HttpSend(QUrl(QString(string).arg(socket->writecard_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
                OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->LK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETZXSTR)
            {

            }

         }
         else if(base_arg.at(0)=="error")
         {
             if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::CASHPAY)
             {
                 OperateFile::tracelog("电力交易失败:"+QString(m_HttpData.at(1)));
                 cancelPay(QString(m_HttpData.at(1)));
             }
             //交费成功，写卡失败
             else
             {
                disconnectSlots();
                messagebox->closeDialog();
                messagebox->display(this,"写卡失败，请到柜台完成写卡");
                socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
                form_dealfinish->init(3);
             }
         }
    }
    else{

           OperateFile::tracelog(QString(NETWORKERROR)+"in inputmount");
           cancelPay("network error");
        }
      reply->deleteLater();
}
void CashPay::replyFinished_writecard(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         OperateFile::tracelog(QString(__func__)+"Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::FKDATASEARCH)
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

           OperateFile::tracelog("写卡数据查询"+QString::fromLocal8Bit(m_receivedata));
           disconnectSlots();
           messagebox->closeDialog();
           messagebox->display(this,"写卡失败，请到柜台完成写卡");
           socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
           form_dealfinish->init(3);
        }
     }
    else
    {

        OperateFile::tracelog(tr("写卡数据查询:")+tr("服务器访问出错"));
        cancelPay("network error");
    }
    reply->deleteLater();
}
void CashPay::cashPay_DealData(KIND reply)
{

}
void CashPay::readywriteCard()
{
    switch (socket->CardType){
    case 0:
    case 1:
    case 3:
        socket->sendMsg("Network&9019&1&EC_GetState");
        break;
    case 2:
        messagebox->closeDialog();
        disconnectSlots();
        socket->effect->begin(this, form_dealfinish,LEFTTORIGHT,NONE,HIDE);
        form_dealfinish->init(0);
        break;
    default:
        break;
    }
}
void CashPay::on_pushButton_clicked()
{
    if(totalsum!=0)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您已放入现金，请继续放钞或点击结束放钞按钮");
        return;
    }
    else{

           messagebox->displayWithoutBlock(this,"停止放钞中……");
           socket->sendMsg("Pipe&PipeBill&1&end");
     }

}
void CashPay::check_CurrenMoney()
{
    OperateFile::tracelog("check current sum");
    timer->stop();
    IsAccaptResponse=true;
    socket->sendMsg("Pipe&PipeBill&1&accept");
}
void CashPay::check_TotalMoney()
{
    timer->stop();

}
void CashPay::GetSerialNumber()
{
    QString data = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = 100000+qrand()%(999999-100000);
    dllsh=data+QString::number(n);
    socket->message.dllsh=dllsh;
}
void CashPay::writecard()
{
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_form_bill()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_from_EC()),Qt::UniqueConnection);    
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
void CashPay::on_pushButton_beginpay_clicked()
{
    /*timer->stop();

    startTimer();
    if(totalsum==0)
       messagebox->displayWithoutBlock(this,"停止放钞中……");
    else
       messagebox->displayWithoutBlock(this,"正在交易，请稍后……");
    socket->sendMsg("Pipe&PipeBill&1&end");*/
    timer->stop();
    startTimer();
    if(totalsum==0)
       messagebox->displayWithoutBlock(this,"停止放钞中……");
    else
       messagebox->displayWithoutBlock(this,"正在交易，请稍后……");
    if(IsAccaptResponse==false)
    {
        BILLCHECKSTATE=LASCHECK;
        timer->start(100);
        OperateFile::tracelog("===========send last check============");
    }
    else
    {
        BILLCHECKSTATE=SECCHECK;
        OperateFile::tracelog("===========wait for tcp response============");

    }

}
void CashPay::backHomeBroken()
{
    disconnectSlots();
    socket->DataBase.insertData(socket->message.yhdabh,socket->message.yhmc,"null",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),QString(totalsum),"0","0",m_errorstr);
    OperateFile::tracelog("交易失败："+QString(m_errorstr));
    form_dealfinish->deleteLater();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->dealDeviceBroken(JSPBROKEN);
}
void CashPay::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_from_EC()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_form_bill()));
}
void CashPay::cancelPay(const QString &Errorstr)
{
    messagebox->closeDialog();
    m_errorstr=Errorstr;
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_form_bill()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(cashPay_GetData_from_EC()),Qt::UniqueConnection);
    socket->sendMsg("Network&9008&1&Printer_GetState");
}
void CashPay::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_7->show();
}
void CashPay::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=PAYSEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_7->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
void CashPay::startTimer()
{
    m_currenttime=PAYSEC;
    ui->lcdNumber->display(PAYSEC);
    ui->lcdNumber->hide();
    ui->label_7->hide();
    emit start(MSEC);
}
void CashPay::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       if(ui->pushButton_beginpay->isEnabled()==true)
        {
           startTimer();
           on_pushButton_beginpay_clicked();
        }
       else
       {
            OperateFile::tracelog("交易超时");
            cancelPay("交易超时");
       }
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void CashPay::connectError(QAbstractSocket::SocketError socketError)
{
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"UMP未启动！");
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE);
        OperateFile::ui_homepage->init();
    }
}

void CashPay::on_pushButton_purchase_clicked()
{
    if(totalsum!=0)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您已放入现金，请继续放钞或点击结束放钞按钮");
        return;
    }
    else{

           messagebox->displayWithoutBlock(this,"停止放钞中……");
           socket->sendMsg("Pipe&PipeBill&1&end");
     }
}

void CashPay::on_pushButton_home_clicked()
{
    if(totalsum!=0)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您已放入现金，请继续放钞或点击结束放钞按钮");
        return;
    }
    else{

           messagebox->displayWithoutBlock(this,"停止放钞中……");
           socket->sendMsg("Pipe&PipeBill&1&end");
     }

}

void CashPay::on_pushButton_search_clicked()
{
    if(totalsum!=0)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"您已放入现金，请继续放钞或点击结束放钞按钮");
        return;
    }
    else{

           messagebox->displayWithoutBlock(this,"停止放钞中……");
           socket->sendMsg("Pipe&PipeBill&1&end");
     }

}

void CashPay::on_pushButton_public_clicked()
{

}
