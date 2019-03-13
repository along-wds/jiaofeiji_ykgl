#include "identifycode.h"
#include "ui_identifycode.h"
#include "homepage.h"
#include "webpage.h"
#include "lookup.h"
#include "msgconfirm.h"
#include "choiceuser.h"
IdentifyCode::IdentifyCode(QString telnumber, QString code, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IdentifyCode),TelNumber(telnumber),CurrentCode(code)
{
    ui->setupUi(this);
    ui->pushButton_send->setStyleSheet("QPushButton{border:1px solid  white;color:white;background-color: transparent}");
    ui->frame->setObjectName("frame2");
    ui->frame_5->setObjectName("frame5");
    ui->frame->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/10+.png);}");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");

    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->lineEdit->setPlaceholderText("验证码");
    ui->lineEdit->setStyleSheet("font-size: 25px;");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat =ui->lcdNumber->palette();
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
    ui->lcdNumber->setPalette(lcdpat);
}

IdentifyCode::~IdentifyCode()
{
    delete ui;
}
void IdentifyCode::init()
{
    ui->lineEdit->setMaxLength(6);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    //startTimer();
    emit stop();
    ui->lcdNumber->hide();
    ui->label_10->hide();
    ui->pushButton_send->setEnabled(false);
    m_currenttime=60;
    ui->pushButton_send->setText(QString::number(m_currenttime));
    emit updateDispaly();
}
void IdentifyCode::on_pushButton_1_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"1");
}

void IdentifyCode::on_pushButton_2_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"2");
}

void IdentifyCode::on_pushButton_3_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"3");
}

void IdentifyCode::on_pushButton_4_clicked()
{   mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"4");
}

void IdentifyCode::on_pushButton_5_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"5");
}

void IdentifyCode::on_pushButton_6_clicked()
{
   mousePressEvent(NULL);
   ui->lineEdit->setText(ui->lineEdit->text()+"6");
}

void IdentifyCode::on_pushButton_7_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"7");
}

void IdentifyCode::on_pushButton_8_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"8");
}

void IdentifyCode::on_pushButton_9_clicked()
{
    mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"9");
}

void IdentifyCode::on_pushButton_0_clicked()
{
     mousePressEvent(NULL);
     ui->lineEdit->setText(ui->lineEdit->text()+"0");
}
void IdentifyCode::on_pushButton_00_clicked()
{
    mousePressEvent(NULL);
    ui->lineEdit->setText(ui->lineEdit->text()+"x");
}
void IdentifyCode::on_pushButton_BackSapce_clicked()
{
    mousePressEvent(NULL);
    if(ui->lineEdit->text().isEmpty())
        return;
    QString text=ui->lineEdit->text();
    text.chop(1);
    ui->lineEdit->setText(text);
}
void IdentifyCode::on_pushButton_Reset_clicked()
{
    mousePressEvent(NULL);
    if(ui->lineEdit->text().isEmpty())
        return;
    ui->lineEdit->setText("");
}
void IdentifyCode::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_10->show();
}
void IdentifyCode::mousePressEvent(QMouseEvent *event)
{
   /*Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->lcdNumber->hide();
   ui->label_10->hide();
   emit start(MSEC);*/
}
void IdentifyCode::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_10->hide();
    emit start(MSEC);

}
void IdentifyCode::setLcdnum()
{
    if(ui->pushButton_send->isEnabled()) //启用页面倒计时
    {
        if(ui->lcdNumber->value()==0)
        {
           messagebox->closeDialog();
           disconnectslots();
           socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,CLOSE,NEIYE);
           OperateFile::ui_homepage->init();
        }
        else
        {
            m_currenttime--;
            ui->lcdNumber->display(m_currenttime);
        }
    }
    else  //启用按钮倒计时
    {
        if(ui->pushButton_send->text().toInt()==0)
        {
            ui->pushButton_send->setEnabled(true);
            ui->pushButton_send->setText("重新发送");
            startTimer();
        }
        else
        {
            m_currenttime--;
            ui->pushButton_send->setText(QString::number(m_currenttime));
        }
    }
}
void IdentifyCode::disconnectslots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void IdentifyCode::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void IdentifyCode::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=true;
}

void IdentifyCode::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=false;

}

void IdentifyCode::on_pushButton_public_clicked()
{
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_webpage,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_webpage->init();

}
//返回
void IdentifyCode::on_pushButton_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectslots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
void IdentifyCode::replyFinished(QNetworkReply *reply)
{
    messagebox->closeDialog();
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[1024];
         memset(m_receivedata,0,1024);
         reply->read(m_receivedata,1024);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
         QStringList base_arg;
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
                if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDXYZM)
                 {
                    //重新发送验证码
                 }
                else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETDETAILMSG)
                 {
                           messagebox->closeDialog();
                           socket->message={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14),m_HttpData.at(15),
                                            m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21),m_HttpData.at(22),m_HttpData.at(23),m_HttpData.at(24),m_HttpData.at(25),m_HttpData.at(26),m_HttpData.at(27),m_HttpData.at(28),m_HttpData.at(29),m_HttpData.at(30),m_HttpData.at(31),
                                            m_HttpData.at(32),m_HttpData.at(33),m_HttpData.at(34),m_HttpData.at(35),m_HttpData.at(36),m_HttpData.at(37),m_HttpData.at(38),m_HttpData.at(39),m_HttpData.at(40),m_HttpData.at(41),m_HttpData.at(42),m_HttpData.at(43),m_HttpData.at(44),m_HttpData.at(45),m_HttpData.at(46),m_HttpData.at(47)};
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
                           ui->lineEdit->setText(socket->message.yhdabh);
                           socket->effect->begin(this, OperateFile::ui_msgconfirm,LEFTTORIGHT,NONE,CLOSE);
                           OperateFile::ui_msgconfirm->init();
                    }
                     else if(socket->IsPurchase==false)
                     {

                         if(OperateFile::ui_lookup==0)
                         {
                             OperateFile::ui_lookup=new Lookup();
                         }
                        disconnectslots();
                        socket->effect->begin(this,OperateFile::ui_lookup,LEFTTORIGHT,NONE,CLOSE);
                        OperateFile::ui_lookup->init();
                     }
                }

             }
             else if(base_arg.at(0)=="error")
             {
                    OperateFile::tracelog("in identifycode:"+QString(m_HttpData.at(1)));
                    QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                    messagebox->displayWithoutButton(this,m_HttpData.at(1));
             }
         }
         else //多表用户
         {
             disconnectslots();
             ChoiceUser *choiceuser=new ChoiceUser(this->getUserInfo());
             choiceuser->setAttribute(Qt::WA_DeleteOnClose);
             socket->effect->begin(this,choiceuser,LEFTTORIGHT,NONE,HIDE);
             choiceuser->init();
         }
    }
    else
    {
        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
    }
    reply->deleteLater();
}

void IdentifyCode::on_pushButton_send_clicked()
{
    emit stop();
    ui->lcdNumber->hide();
    ui->label_10->hide();
    ui->pushButton_send->setEnabled(false);
    m_currenttime=60;
    ui->pushButton_send->setText(QString::number(m_currenttime));
    emit updateDispaly();
    QString CheckConentUrl;
    OperateFile::readiniFile("INTERFACE.DATA","interface/GETDXYZM",CheckConentUrl);
    OperateFile::getIdentifyCode(CurrentCode,6);
    socket->HttpSend(QUrl(QString(CheckConentUrl).arg(TelNumber).arg(CurrentCode)));
    OperateFile::tracelog(QString("Send jsp:\t")+QString(CheckConentUrl).arg(TelNumber).arg(CurrentCode));
}

void IdentifyCode::on_pushButton_OK_clicked()
{
    if(ui->lineEdit->text()==CurrentCode) //验证码正确
    {
        messagebox->displayWithoutBlock(this,"正在获取信息……");
        QString GetInfoUrl;
        OperateFile::readiniFile("INTERFACE.DATA","interface/GETUSERMSG",GetInfoUrl);
        socket->HttpSend(QUrl(QString(GetInfoUrl).arg(TelNumber).arg("2").arg("").arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh)));
        OperateFile::tracelog(QString("Send jsp:\t")+QString(GetInfoUrl).arg(TelNumber).arg("2").arg(socket->FK_msg.dbbh).arg("").arg(socket->LoginMessage.rybh).arg(socket->LoginMessage.gsbh).arg(socket->LoginMessage.fgsbh));
    }
    else
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"验证码输入有误，请重新输入");
    }//验证码输入有误
}
