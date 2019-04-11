#include"inputamount.h"
#include "ui_inputamount.h"
#include"dealfinish.h"
#include"unionpay.h"
#include"readcard.h"
#include"inputpassword.h"
#include"lookup.h"
#include <QHostInfo>
extern REGIST regist;
extern INITDEV initdev;
extern GETSTATE getstate;
extern READCARD read;
extern CLOSEDEV closedev;
extern OPENFEYBOARD openkeyboard;
extern INPUTPWD inputpwd;
extern CLEARPWD clearpwd;
extern CLOSEKEYBOARD closekeyboard;
extern DEAL deal;
extern CANCEL cancelpay;
inputamount::inputamount(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::inputamount)
{
    ui->setupUi(this);
    form_dealfinish=new dealfinish();
    form_dealfinish->setAttribute(Qt::WA_DeleteOnClose);
    ui->frame->setObjectName("frame1");
    ui->frame_6->setObjectName("frame5");
    ui->frame->setStyleSheet("QFrame#frame1{border-image: url(:/image/picture/qietu/10.png);}");
    ui->frame_6->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");

    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                  "QPushButton:disabled{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->lineEdit->setStyleSheet("font-size: 24px;");
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText("购电金额");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    connect(this,SIGNAL(electricDealwithUnion(QString)),socket->unionThread,SLOT(dealwithUnion(QString)),Qt::QueuedConnection);
    connect(socket->unionThread,SIGNAL(electricDealFinished(QString)),this,SLOT(getRespcode(QString)),Qt::QueuedConnection);
}
inputamount::~inputamount()
{
    delete ui;
}
void inputamount::init()
{
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(inputamount_GetData()),Qt::UniqueConnection);
    startTimer();
    ui->pushButton_purchase->setEnabled(false);
}
void inputamount::inputamount_GetData()
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
            CardType=QString(list.at(4)).toInt();
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(CardType)));
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
            OperateFile::tracelog("inputmount send:"+ QString(lk_str).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg(socket->amount_str).arg("0").arg("").arg(socket->Card_Basemsg.FileDate1));
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

   else if(list.at(1)=="getstate")
   {
        if(list.at(2)=="ok"||list.at(3)=="1003")
        {
            QString PrintString;
            QString CurrentTime,currentdevid;
            OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",currentdevid);
            if((socket->OutData.rescode)=="00")
                yl_result="失败";
            else
                yl_result="成功";
            CurrentTime=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
            PrintString.append("        内蒙古电力自助缴费终端消费凭条\r\n");
            PrintString.append("----------------------------------------------\r\n");
            PrintString.append("用户名称:"+socket->message.yhmc+"\r\n");
            PrintString.append("用户编号:"+socket->message.yhdabh+"\r\n");
            PrintString.append("缴费金额:"+ui->lineEdit->text()+" 元"+"\r\n");
            PrintString.append("  终端号:"+currentdevid+"\r\n");
            PrintString.append("  流水号:"+dllsh+"\r\n");
            PrintString.append("缴费方式:自助缴费终端_银行卡\r\n");
            PrintString.append("银联扣款:"+yl_result+"\r\n");
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
void inputamount::on_pushButton_1_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
    ui->lineEdit->setText(ui->lineEdit->text()+"1");
    maxCheck();
}
void inputamount::on_pushButton_2_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
    ui->lineEdit->setText(ui->lineEdit->text()+"2");
    maxCheck();
}

void inputamount::on_pushButton_3_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
    ui->lineEdit->setText(ui->lineEdit->text()+"3");
    maxCheck();
}

void inputamount::on_pushButton_4_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"4");
     maxCheck();
}

void inputamount::on_pushButton_5_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
    ui->lineEdit->setText(ui->lineEdit->text()+"5");
    maxCheck();
}

void inputamount::on_pushButton_6_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"6");
     maxCheck();
}

void inputamount::on_pushButton_7_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"7");
     maxCheck();
}

void inputamount::on_pushButton_8_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
    ui->lineEdit->setText(ui->lineEdit->text()+"8");
    maxCheck();
}

void inputamount::on_pushButton_9_clicked()
{
    startTimer();
    if(-1==bitCheck())
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"9");
     maxCheck();
}

void inputamount::on_pushButton_0_clicked()
{
    startTimer();
    if (ui->lineEdit->text() =="0")
        return;
    if(-1==bitCheck())
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"0");
     maxCheck();
}

void inputamount::on_pushButton_BackSapce_clicked()
{
    startTimer();
    if(ui->lineEdit->text()=="0")
        return;
    QString text=ui->lineEdit->text();
    text.chop(1);
    if (text.isEmpty())
        ui->lineEdit->setText("0");
    else
        ui->lineEdit->setText(text);
}

void inputamount::on_pushButton_Reset_clicked()
{
    startTimer();
    if(ui->lineEdit->text().isEmpty())
        return;
    ui->lineEdit->setText("0");
}
void inputamount::on_pushButton_11_clicked()
{
    startTimer();
    if (!ui->lineEdit->text().contains(".")&&ui->lineEdit->text()!="")
        ui->lineEdit->setText(ui->lineEdit->text() + tr("."));
}
void inputamount::on_pushButton_00_clicked()
{
    if (ui->lineEdit->text() == "")
        return;
    if (ui->lineEdit->text() =="0")
        return;
     ui->lineEdit->setText(ui->lineEdit->text()+"00");
}
//点击确定按钮
void inputamount::on_pushButton_Ok_clicked()
{
    //普通户缴费金额限制
    /*if(socket->message.fffs=="01")
    {
       if(socket->message.total.toInt()>10&&fabs(socket->message.total.toDouble())>fabs(ui->lineEdit->text().toFloat()))
       {
           QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,QString("交费金额不能低于%1元!").arg(QString::number(socket->message.total.toDouble(),'f',2)));
           ui->lineEdit->clear();
           ui->lineEdit->setText("0");
           return;
       }
    }*/
   /***********************/
   startTimer();
   socket->amount_str=ui->lineEdit->text();
   ui->pushButton_Ok->setEnabled(false);
   /***************************/
   messagebox->displayWithoutBlock(this,"正在交易，请稍后……");//等待提示 QElapsedTimer t;

   /*************/
#ifdef DEBUG
   strcpy(socket->InData.number,"123****456");
#endif
   QString string,e,tpdu;
   GetSerialNumber();
   OperateFile::readiniFile(DEVINFO,"TERMINF/MCHT_ID",tpdu);
   OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",e);
   OperateFile::readiniFile("INTERFACE.DATA",QString("UNIFYPLATFORM/UnifyPlatformPath_deal"),string);
   socket->HttpSend(QUrl(QString(string).arg(dllsh).arg(socket->LoginMessage.yyzbh).arg(socket->message.yhdabh).arg("").arg(e).arg(tpdu).arg(ui->lineEdit->text()).arg("yl").arg(QString(QString("银联").toLocal8Bit().toPercentEncoding())).arg("01").arg(QString(socket->InData.number)).arg("024").arg(QString(QString("自助缴费-刷卡").toLocal8Bit().toPercentEncoding()))));
   OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(dllsh).arg(socket->LoginMessage.yyzbh).arg(socket->message.yhdabh).arg("").arg(e).arg(tpdu).arg(ui->lineEdit->text()).arg("yl").arg(QString(QString("银联").toLocal8Bit().toPercentEncoding())).arg("01").arg(QString(socket->InData.number)).arg("024").arg(QString(QString("自助缴费-刷卡").toLocal8Bit().toPercentEncoding())));
   /**************/
   /*QElapsedTimer t1;
   t1.start();
   while(t1.elapsed()<800)
      QCoreApplication::processEvents();
   emit electricDealwithUnion(ui->lineEdit->text());*/
}
void inputamount::getRespcode(QString amount)
{
   m_je=amount;
   if((socket->OutData.rescode=="00"))
   {
       QString inistring;
       //发送更新接口

       OperateFile::readiniFile("INTERFACE.DATA",QString("UNIFYPLATFORM/UnifyPlatformPath_dealUpdate"),inistring);
       socket->HttpSend(QUrl(QString(inistring).arg(m_je).arg(dllsh).arg(QString(socket->OutData.trace)).arg(QString(socket->OutData.rescode)).arg(QString(socket->OutData.bank_code)).arg(QString(socket->OutData.card_no.trimmed())).arg(QString(socket->OutData.batch)).arg(QString(QString(socket->OutData.resp_chin.trimmed()).toLocal8Bit().toPercentEncoding())).arg(QString(QString(socket->OutData.info.trimmed()).toLocal8Bit().toPercentEncoding())).arg(QString(socket->OutData.reference)).arg(QString(socket->OutData.expr))));
       OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg(m_je).arg(dllsh).arg(QString(socket->OutData.trace)).arg(QString(socket->OutData.rescode)).arg(QString(socket->OutData.bank_code)).arg(QString(socket->OutData.card_no.trimmed())).arg(QString(socket->OutData.batch)).arg(QString(QString(socket->OutData.resp_chin.trimmed()).toLocal8Bit().toPercentEncoding())).arg(QString(QString(socket->OutData.info.trimmed()).toLocal8Bit().toPercentEncoding())).arg(QString(socket->OutData.reference)).arg(QString(socket->OutData.expr)));
   }
   else
   {
       messagebox->closeDialog();
       QString string;
       if((socket->OutData.rescode=="33")||(socket->OutData.rescode=="35")||(socket->OutData.rescode=="36")||(socket->OutData.rescode=="37")||(socket->OutData.rescode=="41")||(socket->OutData.rescode=="43")||(socket->OutData.rescode=="67"))
       {
            //cardswallow();
            disconnectSlots();
            OperateFile::readiniFile("rsp.ini",QString("RESPCODE/")+QString(socket->OutData.rescode),string);
            QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"受限制的卡，卡已被没收");
            //messagebox->display(this,"受限制的卡，卡已被没收");
            OperateFile::tracelog(string);

            OperateFile::ui_inputpassword->close();
            form_dealfinish->deleteLater();
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
            OperateFile::ui_homepage->ejectCard();
            //OperateFile::ui_homepage->init();
       }
       else if((socket->OutData.rescode=="51"))
       {
          // messagebox->display(this,"卡内账户余额不足");
           disconnectSlots();
           QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"卡内账户余额不足");

           OperateFile::ui_inputpassword->close();
           form_dealfinish->deleteLater();
           socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
           OperateFile::ui_homepage->ejectCard();
           //OperateFile::ui_homepage->init();
       }
       else if((socket->OutData.rescode=="55"))
       {
           disconnectSlots();
          // messagebox->display(this,"密码输入有误，请重新输入");
           QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"密码输入有误，请重新输入");
           form_dealfinish->deleteLater();
           socket->effect->begin(this, OperateFile::ui_inputpassword,LEFTTORIGHT,NONE,CLOSE);
           //OperateFile::ui_inputpassword->init();
           QElapsedTimer t;
           t.start();
           while(t.elapsed()<800)
               QCoreApplication::processEvents();
           OperateFile::ui_inputpassword->beginInput();
       }
       else if((socket->OutData.rescode=="A0"))
       {
           //messagebox->display(this,"交易异常，为了您的账户安全建议您重新插入银行卡");
           disconnectSlots();
           QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"交易异常，为了您的账户安全建议您重新插入银行卡");
           OperateFile::ui_inputpassword->close();
           form_dealfinish->deleteLater();
           socket->effect->begin(this, OperateFile::ui_unionpay,RIGHTTOLEFT,NONE,CLOSE);
           OperateFile::ui_homepage->ejectCard();
           //OperateFile::ui_unionpay->init();
           OperateFile::ui_unionpay->registtoUnion();
       }
       else
       {
            OperateFile::readiniFile("rsp.ini",QString("RESPCODE/")+QString(socket->OutData.rescode),string);
            if(QString(socket->OutData.rescode).toInt()==0)
            {
                QTimer::singleShot(5000, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"扣款失败，请到柜台完成缴费");
            }
            else
            {
                //messagebox->display(this,string);
                QTimer::singleShot(5000, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,string);
            }
            disconnectSlots();
            OperateFile::ui_inputpassword->close();
            form_dealfinish->deleteLater();
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
            OperateFile::ui_homepage->ejectCard();
            //OperateFile::ui_homepage->init();
       }
 }
 }
void inputamount::replyFinished(QNetworkReply *reply)
{
    QString string;
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         reply->read(m_receivedata,4096);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::UNIONPAY)
            {
                socket->YL_msg={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10)};
                socket->DataBase.insertData(socket->message.yhdabh,socket->message.yhmc,"",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),socket->YL_msg.sjje,"1","1","");

                readywriteCard();
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::CASHPAY)
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
                OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->writecard_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));
            }

         }
         else if(base_arg.at(0)=="error")
         {

             if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::UNIONPAY)
             {
                 OperateFile::tracelog("电力交易失败:"+QString(m_HttpData.at(1)));
                 cancelPay(QString(m_HttpData.at(1)));
             }
             //写卡失败，缴费完成
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
void inputamount::replyFinished_writecard(QNetworkReply *reply)
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
            messagebox->display(this,QString(m_HttpData.at(1)));
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
double inputamount:: round(double d, unsigned p)
{
    if(d==0)
        return 0;
    double n=1;
    for(;p>0;p--)
        n*=10;
    if(d>0.0)
        if(d-(__int64)(d*n)/n>=5)
            return (__int64)(d*n+0.5) / n;
        else
            return (__int64)(d*n+0.5000001) / n;
    else
        if(d-(__int64)(d*n)/n>=5)
            return (__int64)(d*n-0.5) / n;
        else
            return (__int64)(d*n-0.5000001) / n;
}
void inputamount::readywriteCard()
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
void inputamount::on_pushButton_back_homepage_clicked()
{
    ui->pushButton_back_homepage->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    form_dealfinish->deleteLater();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->ejectCard();
    //OperateFile::ui_homepage->init();
}
void inputamount::writecard()
{
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
void inputamount::cancelPay(const QString &Errorstr)
{
    messagebox->closeDialog();
    m_errorstr=Errorstr;
    socket->sendMsg("Network&9008&1&Printer_GetState");
}
void inputamount::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_4->show();
}
void inputamount::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=PAYSEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_4->hide();
    emit start(MSEC);
}
void inputamount::startTimer()
{
    m_currenttime=PAYSEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_4->hide();
    emit start(MSEC);
}
void inputamount::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       if(ui->pushButton_Ok->isEnabled()==true)
        {
            disconnectSlots();
            OperateFile::ui_inputpassword->close();
            form_dealfinish->deleteLater();
            OperateFile::ui_homepage->ejectCard();
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
            //OperateFile::ui_homepage->init();
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
//交易撤销函数
/*void inputamount::begincancelPay()
{
    uchar stat_out;
    if(unioninit(1)!=0)
     {
        OperateFile::tracelog("in cancelPay:银联初始化失败");
        messagebox->displayWithoutButton(this,"交易取消取失败，请联系柜台");
     }
    //检测卡
    checkcard(&stat_out);

    if(stat_out!=0x37)
    {
        OperateFile::tracelog("in cancelPay:卡不在读卡器内");
        messagebox->displayWithoutButton(this,"交易取消失败，请联系柜台");
    }
    //读卡
    if(0!=read(socket->InData.number))
    {
        OperateFile::tracelog("in cancelPay:读卡失败");
        messagebox->displayWithoutButton(this,"交易取消失败，请联系柜台");
    }
    QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
    messagebox->displayWithoutButton(this,"请再次输入密码以取消交易");
    socket->effect->begin(this, OperateFile::ui_inputpassword,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_inputpassword->init();
    socket->IscancelDeal=true;
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<800)
        QCoreApplication::processEvents();
    //qDebug()<<"输入密码";
    OperateFile::ui_inputpassword->beginInput();
}*/
void inputamount::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(inputamount_GetData()));
}
/***限制小数点位数***/
int inputamount::bitCheck()
{
    if( ui->lineEdit->text().contains("."))
    {
        QString str=ui->lineEdit->text().section(".",1,1);
        if(str.size()>1)
            return -1;
    }
    if(ui->lineEdit->text()=="0")
        ui->lineEdit->clear();
    return 0;
}
int inputamount::maxCheck()
{
    if(ui->lineEdit->text().length()>10)
    {
        QString text=ui->lineEdit->text();
        text.chop(1);
        ui->lineEdit->setText(text);
    }
    return 0;
}
void inputamount::replyFinished_unifyplatform(QNetworkReply *reply)
{
    QString inistring;
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         OperateFile::tracelog("unifyplatform:"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(base_arg.at(0)=="ok")
         {
            if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
            {
                 OperateFile::tracelog("error argument");
                 return;
            }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::DEALLOGINSERT&&QString(m_HttpData.at(1))=="0")
            {
                 qDebug()<<"electricDealwithUnion";
                 emit electricDealwithUnion(ui->lineEdit->text());
            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::DEALLOGUPDATE&&QString(m_HttpData.at(1))=="0")
            {

                 disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
                 connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
                 // 收费保存
#ifndef DEBUG
                 OperateFile::readiniFile("INTERFACE.DATA","interface/CHARGE",inistring);
                 socket->HttpSend(QUrl(QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                               .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                               .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                               .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh).arg(dllsh).arg(QString(socket->OutData.trace)).arg(socket->OutData.stermid)));

                 OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                       .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                       .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                                       .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh).arg(dllsh).arg(QString(socket->OutData.trace).arg(socket->OutData.stermid)));
#else


                OperateFile::readiniFile("INTERFACE.DATA","interface/CASHPAY",inistring);
                socket->HttpSend(QUrl(QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                                .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh)));

                OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg("").arg(socket->message.ydhy).arg("null").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                      .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                      .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("null").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg("_").arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("null")
                                      .arg(socket->message.phone).arg("null").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh));

            /*

                 OperateFile::readiniFile("INTERFACE.DATA","interface/CHARGE",inistring);
                 socket->HttpSend(QUrl(QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                               .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                               .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                               .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh).arg(dllsh).arg(QString(socket->OutData.trace))));

                 OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg("").arg(socket->message.ydhy).arg("").arg("").arg(socket->message.path).arg(socket->message.copierid).arg(socket->message.urgeid).arg(socket->message.accountid).arg(socket->message.gsbh)
                                       .arg(socket->message.fgsbh).arg(socket->message.yyzbh).arg(socket->message.yhdabh)
                                       .arg(ui->lineEdit->text()).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(socket->message.yhzmbh).arg("").arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh).arg(socket->LoginMessage.ip).arg(socket->LoginMessage.yyzbh).arg(socket->message.wyj).arg(socket->message.zhye).arg(socket->message.yzje).arg(socket->message.xzje).arg(socket->message.sfznum).arg("")
                                       .arg(socket->message.phone).arg("").arg(socket->message.jlbbh).arg(socket->message.sh).arg(socket->message.total).arg(socket->message.fffs).arg("").arg(QString::number(socket->CardType)).arg(socket->message.fffs).arg(socket->message.ickid).arg(socket->message.xgdj).arg(socket->message.jlxh).arg(dllsh).arg(QString(socket->OutData.trace)));
            */
#endif
            }
            else //接口调用失败返回
            {
                OperateFile::tracelog("一体化消费更新接口调用失败");
                cancelPay("一体化消费更新接口调用失败");
            }
         }
         else if(base_arg.at(0)=="error")
         {
               OperateFile::tracelog("一体化消费更新接口调用失败");
               cancelPay("一体化消费更新接口调用失败");
         }
    }
    else
    {
        OperateFile::tracelog(QString(NETWORKERROR)+"in inputmount_unif networkerror");
        cancelPay(QString(NETWORKERROR));
    }
    reply->deleteLater();
}
//生成银联流水号
void inputamount::GetSerialNumber()
{
    QString data = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = 100000+qrand()%(999999-100000);
    dllsh=data+QString::number(n);
}
/***********接口异常返回******************/
void inputamount::backHomeBroken()
{
    disconnectSlots();
    socket->DataBase.insertData(socket->message.yhdabh,socket->message.yhmc,"null",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),ui->lineEdit->text(),"1","0",m_errorstr);
    OperateFile::tracelog("交易失败："+QString(m_errorstr));
    OperateFile::ui_inputpassword->close();
    form_dealfinish->deleteLater();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->dealDeviceBroken(JSPBROKEN);
}
void inputamount::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    form_dealfinish->deleteLater();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->ejectCard();
    //OperateFile::ui_homepage->init();
}

void inputamount::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    form_dealfinish->deleteLater();
    socket->IsPurchase=true;
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_readcard,RIGHTTOLEFT,NONE,HIDE);
    //OperateFile::ui_readcard->init();
}

void inputamount::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    form_dealfinish->deleteLater();
    if(OperateFile::ui_lookup==0)
    {
        OperateFile::ui_lookup=new Lookup();
    }
    socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
    //OperateFile::ui_lookup->init();

}
void inputamount::on_pushButton_public_clicked()
{

}
