#include "khxx.h"
#include "ui_khxx.h"
#include"homepage.h"
#include"first.h"
#include"manager.h"
#include"lookup.h"
#include"readcard.h"
KHxx::KHxx(QWidget *parent):
    QWidget(parent),
    ui(new Ui::KHxx)
{
    ui->setupUi(this);
    ui->frame_2->setObjectName("frame1");
    ui->frame_5->setObjectName("frame5");
    ui->frame_2->setStyleSheet("QFrame#frame1{background-color:#dfdfdf;border:1px;border-radius:3px;}");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase.png);}");
    ui->pushButton_search->setEnabled(false);
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search+.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}"
                                    "QPushButton:disabled{border-image: url(:/image/picture/qietu/search+.png);}");

    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
     //调色板
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);

    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QString string;
    OperateFile::readiniFile("INTERFACE.DATA","interface/GETKHXX",string);
    socket->HttpSend(QUrl(QString(string).arg(socket->message.yhdabh)));
    OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->message.yhdabh));

    startTimer();
}
void KHxx::on_pushButton_back_2_clicked()
{
    ui->pushButton_back_2->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_lookup,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_lookup->init();
}
void KHxx::disconnectSlots()
{
    emit stop();
     disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
     disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
     disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
     disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
     disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));

}
KHxx::~KHxx()
{
    delete ui;
}

void KHxx::display()
{
    int namesize=socket->message.yhmc.size();
    int phonesize=socket->message.phone.size();
    int sfzhsize=socket->message.sfznum.size();
    QString Displayname=socket->client_msg.yhmc;
    QString Displayphone=socket->client_msg.tel;
    QString Displaysfzh=socket->client_msg.zjhm;
    if(namesize!=0)
    {
        Displayname=Displayname.replace(0,1,"*");
    }
    if(phonesize!=0)
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
    ui->label_ClientName->setText(Displayname);
    ui->label_ClientNum->setText(socket->client_msg.yhdabh);
    ui->label_IDNumber->setText(Displaysfzh);
    ui->label_ContactTelephone->setText(Displayphone);
    ui->label_lxr->setText(socket->client_msg.lxr);
    ui->label_fffs->setText(socket->client_msg.fffs);
    ui->label_cblxbh->setText(socket->client_msg.cblxbh);
    ui->label_yyzbh->setText(socket->client_msg.yyzbh);
    ui->label_address->setText(socket->client_msg.addr);
}
void  KHxx::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_9->show();

}
void  KHxx::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_9->hide();
    emit start(MSEC);
}
void  KHxx::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label_9->hide();
    emit start(MSEC);
}
void  KHxx::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        emit stop();
        disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
        disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
        disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
        disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
        disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,CLOSE);
        OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void KHxx::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         //reply->read(m_receivedata,4096);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(reply->readAll().data()).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");

         if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
         {
             OperateFile::tracelog("error argument");
             return;
         }
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETKHXX)
             {
                 socket->client_msg={m_HttpData.at(1),m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12)};
             }
              display();
         }
     }
    else
    {
        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");

    }

}

void KHxx::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void KHxx::on_pushButton_purchase_clicked()
{
    ui->pushButton_purchase->setEnabled(false);
    socket->IsPurchase=true;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void KHxx::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=false;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void KHxx::on_pushButton_public_clicked()
{

}
