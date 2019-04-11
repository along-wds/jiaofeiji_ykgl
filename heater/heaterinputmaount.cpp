 #include "heaterinputmaount.h"
#include "ui_heaterinputmaount.h"
#include"manager.h"
#include"operatefile.h"
#include<QMessageBox>
#include"first.h"
#include"inputpassword.h"
#include"unionpay.h"
extern ClientSocket *socket;
extern Manager *ui_manager;
Heaterinputmaount::Heaterinputmaount(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Heaterinputmaount)
{
    ui->setupUi(this);
    m_recvtimes=0;
    ui->frame->setObjectName("frame1");
    ui->frame_2->setObjectName("frame2");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/10-.png);}");
    ui->frame->setStyleSheet("QFrame#frame1{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    ui->lineEdit_number->setStyleSheet("font-size: 24px;");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    connect(this,SIGNAL(heaterDealwithUnion(QString)),socket->unionThread,SLOT(dealwithUnion(QString)),Qt::QueuedConnection);
    connect(socket->unionThread,SIGNAL(heaterDealFinished(QString)),this,SLOT(getRespcode(QString)),Qt::QueuedConnection);
}
void Heaterinputmaount::init()
{
    ui->pushButton_Ok_3->setEnabled(true);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(getData()),Qt::QueuedConnection);
    ui->lineEdit_number->setText(socket->m_heatersearchmsg.qjzcnf);
    ui->lineEdit_number->setReadOnly(true);
    startTimer();
}
Heaterinputmaount::~Heaterinputmaount()
{
    delete ui;
}
void Heaterinputmaount::on_pushButton_22_clicked()
{
    if(-1==bitCheck())
        return;
    ui->lineEdit_number->setText(ui->lineEdit_number->text()+"1");
}

void Heaterinputmaount::on_pushButton_23_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"2");
}

void Heaterinputmaount::on_pushButton_20_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"3");
}

void Heaterinputmaount::on_pushButton_21_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"4");
}

void Heaterinputmaount::on_pushButton_29_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"5");
}

void Heaterinputmaount::on_pushButton_25_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"6");
}

void Heaterinputmaount::on_pushButton_26_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"7");
}

void Heaterinputmaount::on_pushButton_24_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"8");
}

void Heaterinputmaount::on_pushButton_27_clicked()
{
    if(-1==bitCheck())
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"9");
}

void Heaterinputmaount::on_pushButton_28_clicked()
{
    if ( ui->lineEdit_number->text() == "0")
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"0");
}
void Heaterinputmaount::on_pushButton_4_clicked()
{
    if (!ui->lineEdit_number->text().contains("."))
        ui->lineEdit_number->setText(ui->lineEdit_number->text() + tr("."));

}
void Heaterinputmaount::on_pushButton_00_clicked()
{
    if (ui->lineEdit_number->text() == "0")
        return;
     ui->lineEdit_number->setText(ui->lineEdit_number->text()+"00");
}
void Heaterinputmaount::on_pushButton_BackSapce_3_clicked()
{
    if(ui->lineEdit_number->text().isEmpty())
        return;
    QString text=ui->lineEdit_number->text();
    text.chop(1);
    ui->lineEdit_number->setText(text);
}
void Heaterinputmaount::on_pushButton_Reset_3_clicked()
{
    if(ui->lineEdit_number->text().isEmpty())
        return;
    ui->lineEdit_number->text().clear();
    ui->lineEdit_number->setText("");

}
int Heaterinputmaount::bitCheck()
{
    if( ui->lineEdit_number->text().contains("."))
    {
        QString str=ui->lineEdit_number->text().section(".",1,1);
        if(str.size()>1)
            return -1;
    }
    return 0;
}
double Heaterinputmaount:: round(double d, unsigned p)
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
void Heaterinputmaount::on_pushButton_back_homepage_clicked()
{
    ui->pushButton_back_homepage->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);

}
void Heaterinputmaount::on_pushButton_Ok_3_clicked()
{
    ui->pushButton_Ok_3->setEnabled(false);
    if (fabs(ui->lineEdit_number->text().toDouble())<0.00001)
    {
        messagebox->display(this,QString::fromLocal8Bit("交费金额不能为0!"));
        return;
    }
    /***********************/
     startTimer();
     ui->pushButton_Ok_3->setEnabled(false);
    /***************************/
   messagebox->displayWithoutBlock(this,"正在交易，请稍后……");//等待提示 QElapsedTimer t;
   QElapsedTimer t1;
   t1.start();
   while(t1.elapsed()<800)
           QCoreApplication::processEvents();
   //qDebug()<<"mainID:"<<QThread::currentThreadId();
   emit heaterDealwithUnion(ui->lineEdit_number->text());
}
void Heaterinputmaount::getRespcode(QString amount)
{
    /*m_je=amount;
    if(socket->OutData.strRespcode=="00")
    {
        // 02 热力  3 自助缴费
         QString string,e;
         GetSerialNumber();
         OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",e);
         OperateFile::readiniFile("INTERFACE.DATA",QString("UNIFYPLATFORM/UnifyPlatformPath_deal"),string);
         socket->HttpSend(QUrl(QString(string).arg(dllsh).arg("02").arg(socket->m_heatersearchmsg.fwbh).arg(QString(socket->m_heatersearchmsg.yhmc.toLocal8Bit().toPercentEncoding())).arg(e).arg(socket->OutData.strMid).arg(amount).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg("02").arg(socket->OutData.strCardNo).arg("3").arg(QString("ATM"))));
         OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(dllsh).arg("02").arg(socket->m_heatersearchmsg.fwbh).arg(socket->m_heatersearchmsg.yhmc).arg(e).arg(socket->OutData.strMid).arg(amount).arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.rymc).arg("02").arg(socket->OutData.strCardNo).arg("3").arg(QString("ATM")));
    }
    else
    {
        QString string;
        messagebox->closeDialog();
        disconnectSlots();
        if(socket->OutData.strRespcode=="33"||socket->OutData.strRespcode=="35"||socket->OutData.strRespcode=="36"||socket->OutData.strRespcode=="37"||socket->OutData.strRespcode=="41"||socket->OutData.strRespcode=="43"||socket->OutData.strRespcode=="67")
        {
             OperateFile::tracelog(socket->OutData.strRespcode+string);
             cardswallow();
             OperateFile::readiniFile("rsp.ini",QString("RESPCODE/")+socket->OutData.strRespcode,string);
             QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
             messagebox->displayWithoutButton(this,"受限制的卡，卡已被没收");
             OperateFile::tracelog(socket->OutData.strRespcode+string);
             OperateFile::ui_inputpassword->close();
             socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
        }
        else if(socket->OutData.strRespcode=="55")
        {
            messagebox->display(this,"密码输入有误，请重新输入");
            socket->effect->begin(this, OperateFile::ui_inputpassword,LEFTTORIGHT,NONE,CLOSE);
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<800)
                QCoreApplication::processEvents();
            OperateFile::ui_inputpassword->init();
            OperateFile::ui_inputpassword->beginInput();
        }
        else if(socket->OutData.strRespcode=="51")
        {
            messagebox->display(this,"卡内账户余额不足");
            OperateFile::ui_inputpassword->close();
            OperateFile::ui_homepage->ejectCard();
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);

        }
        else
        {

            if(socket->OutData.strRespcode.toInt()==0){

               messagebox->display(this,"扣款失败，请到柜台完成缴费");
            }
            else{
                OperateFile::tracelog("respon string"+string);
                OperateFile::readiniFile("rsp.ini",QString("RESPCODE/")+socket->OutData.strRespcode,string);
                messagebox->display(this,string);
            }
            disconnectSlots();
            OperateFile::ui_inputpassword->close();
            OperateFile::ui_homepage->ejectCard();
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
        }
    }*/
}
void Heaterinputmaount::replyFinished(QNetworkReply *reply)
{
    /*//qDebug()<<"httprecive";
    if(reply->error()==QNetworkReply::NoError)
     {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         //qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         if(m_HttpData.at(0)=="ok"&&m_HttpData.size()==9)
         {
              disconnectSlots();
              form_heaterdealfinished=new Heaterdealfinished(m_HttpData);
              form_heaterdealfinished->setAttribute(Qt::WA_DeleteOnClose);
              messagebox->closeDialog();
              socket->effect->begin(this,form_heaterdealfinished,LEFTTORIGHT,NONE,HIDE);

         }

         else
         {
              messagebox->closeDialog();
              OperateFile::tracelog("交易失败:"+QString::fromLocal8Bit(m_receivedata));
              cancelPay(QString::fromLocal8Bit(m_receivedata));
         }
    }
    else
    {
        cancelPay(QString(NETWORKERROR));
    }
    reply->deleteLater();*/
 }
void Heaterinputmaount::replyFinished_unifyplatform(QNetworkReply *reply)
{
   /* QString inistring;
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         m_recvtimes++;
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         QString recvstr=QString::fromLocal8Bit(m_receivedata);
         if(recvstr=="0")
         {
             switch (m_recvtimes) {
             case 1:
                 OperateFile::readiniFile("INTERFACE.DATA",QString("UNIFYPLATFORM/UnifyPlatformPath_dealUpdate"),inistring);
                 socket->HttpSend(QUrl(QString(inistring).arg(m_je).arg(dllsh).arg(socket->OutData.strMemo).arg(socket->OutData.strRespcode).arg(" ").arg(socket->OutData.strCardNo).arg(socket->OutData.strBatch).arg(QString(socket->OutData.strRespInfo.toLocal8Bit().toPercentEncoding())).arg(" ").arg(socket->OutData.strRef).arg(socket->OutData.strTransDta)));
                 OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring).arg(m_je).arg(dllsh).arg(socket->OutData.strMemo).arg(socket->OutData.strRespcode).arg(" ").arg(socket->OutData.strCardNo).arg(socket->OutData.strBatch).arg(QString::fromLocal8Bit(socket->OutData.strRespInfo.toLatin1())).arg(" ").arg(socket->OutData.strRef).arg(socket->OutData.strTransDta));
                 break;
             case 2:
                 disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
                 connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
                 OperateFile::readiniFile("heater.ini","interface/YLJF",inistring);
                 socket->HttpSend(QUrl(QString(inistring.arg(socket->m_heatersearchmsg.fwbh).arg(ui->lineEdit_number->text()).arg(socket->m_heatersearchmsg.fxzq).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLocal8Bit().toPercentEncoding())).arg(dllsh).arg("3"))));
                 OperateFile::tracelog(QString("Send jsp:\t")+QString(inistring.arg(socket->m_heatersearchmsg.fwbh).arg(ui->lineEdit_number->text()).arg(socket->m_heatersearchmsg.fxzq).arg(socket->LoginMessage.rybh).arg(QString::fromLocal8Bit(socket->LoginMessage.rymc.toLatin1())).arg(dllsh).arg("3")));
                 m_recvtimes=0;
                 break;
             default:
                 break;
             }
         }
         else if(recvstr=="1")
         {
             switch (m_recvtimes) {
             case 1:
                 OperateFile::tracelog("一体化对账日志消费接口调用失败");
                 cancelPay("一体化对账日志消费接口调用失败");
                 break;
             case 2:
                OperateFile::tracelog("一体化对账日志消费更新接口调用失败");
                cancelPay("一体化对账日志消费更新接口调用失败");
                m_recvtimes=0;
                 break;
             default:
                 break;
             }
         }
    }
    else
    {
        //服务器拒绝访问
        OperateFile::tracelog(QString(NETWORKERROR)+"in inputmount_unif networkerror");
        cancelPay(QString(NETWORKERROR));
    }
    reply->deleteLater();*/
}
void Heaterinputmaount::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Heaterinputmaount::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);
}
void Heaterinputmaount::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->frame_lcd->hide();
    ui->frame_3->hide();
    emit start(MSEC);
}
void Heaterinputmaount::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        if(ui->pushButton_Ok_3->isEnabled()==true)
         {
             disconnectSlots();
             OperateFile::ui_inputpassword->close();
             OperateFile::ui_homepage->ejectCard();
             socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
         }
        else
        {
            OperateFile::tracelog("in heater deal: 交易超时");
            cancelPay("交易超时");
        }
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void Heaterinputmaount::on_pushButton_3_clicked()
{
    on_pushButton_Ok_3_clicked();
}
/*void Heaterinputmaount::begincancelPay()
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
    qDebug()<<"输入密码";
    OperateFile::ui_inputpassword->beginInput();
}*/
void Heaterinputmaount::cancelPay(const QString &Errorstr)
{
    m_errorstr=Errorstr;
    socket->sendMsg("Network&9008&1&Printer_GetState");
}
void Heaterinputmaount::GetSerialNumber()
{
        QString data = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QTime t;
        t= QTime::currentTime();
        qsrand(t.msec()+t.second()*1000);
        int n = 100000+qrand()%(999999-100000);
        dllsh=data+QString::number(n);
}
void Heaterinputmaount::getData()
{
    /*OperateFile::tracelog(socket->ToString());
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }
    if(list.at(1)=="getstate")
    {
        if(list.at(2)=="ok"||list.at(3)=="1003")
        {
            QString PrintStr;
            QString CurrentTime;

            if(socket->OutData.strRespcode!="00")
                yl_result="失败";
            else
                yl_result="成功";
            CurrentTime=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
            PrintStr=QString("Network&9008&1&Printer_Print_      新疆锦龙电力自助缴费终端热力交款收据\r\n----------------------------------------------\r\n房屋编号：%1\r\n用户名称：%2\r\n缴费金额：%3\r\n缴费日期：%4\r\n  流水号：%5\r\n银联扣款结果：%6\r\n缴费结果：%7\r\n----------------------------------------------\r\n\r\n\r\n\r\n\r\n").arg(socket->m_heatersearchmsg.fwbh).arg(socket->m_heatersearchmsg.yhmc).arg(socket->m_heatersearchmsg.qjzcnf).arg(CurrentTime).arg(socket->OutData.strMemo).arg("失败").arg(yl_result);
            socket->sendMsg(PrintStr);
        }
        else
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            messagebox->display(this,"缴费失败，请到柜台完成缴费");
            backHomeBroken();
        }
    }
    if(list.at(1)=="print")
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
    }*/
}
void Heaterinputmaount::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(getData()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_unifyplatform(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
/***********接口异常返回******************/
void Heaterinputmaount::backHomeBroken()
{
    /*disconnectSlots();
    socket->DataBase.insertData(socket->m_heatersearchmsg.fwbh,socket->m_heatersearchmsg.yhmc,socket->OutData.strMemo,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),ui->lineEdit_number->text(),"热力",yl_result,"失败",m_errorstr);
    OperateFile::tracelog("交易失败："+QString(m_errorstr));
    OperateFile::ui_inputpassword->close();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
    OperateFile::ui_homepage->dealDeviceBroken(JSPBROKEN);*/
}
