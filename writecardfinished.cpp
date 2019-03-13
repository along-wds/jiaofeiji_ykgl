#include "writecardfinished.h"
#include "ui_writecardfinished.h"
#include"homepage.h"
#include"manager.h"
#include "readcard.h"
#include"lookup.h"
WritecardFinished::WritecardFinished(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WritecardFinished)
{
    ui->setupUi(this);
    ui->frame_5->setObjectName("frame5");
    ui->frame_2->setObjectName("frame2");
    ui->frame_8->setObjectName("frame8");
    ui->frame_3->setObjectName("frame3");
    ui->frame_4->setObjectName("frame4");
    ui->frame_4->setStyleSheet("QFrame#frame4{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame_3->setStyleSheet("QFrame#frame3{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame_8->setStyleSheet("QFrame#frame8{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->frame_2->setStyleSheet("QFrame#frame2{background-color:#dfdfdf;border:1px;border-radius:3px;}");
    ui->pushButton_backHomePage_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:disabled{border-image: url(:/image/picture/qietu/purchase.png);}");
    ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_4->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}

WritecardFinished::~WritecardFinished()
{
    delete ui;
}
void WritecardFinished::init()
{
    ui->pushButton->setEnabled(false);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    //connect(socket,SIGNAL(readyRead()),this,SLOT(dealfinish_GetData()),Qt::UniqueConnection);
    startTimer();
    display();
}
void WritecardFinished::display()
{
    /*****************************************/
        //显示内容
    /****************************************/
    int namesize=socket->message.yhmc.size();
    int phonesize=socket->message.phone.size();
    int sfzhsize=socket->message.sfznum.size();
    QString Displayname=socket->message.yhmc;
    QString Displayphone=socket->message.phone;
    QString Displaysfzh;
    if(namesize!=0)
    {
         Displayname=Displayname.replace(0,1,"*");
    }
    if(phonesize!=0)
    {
        Displaysfzh=socket->message.phone.left(3)+socket->message.phone.right(4);
        for(int i=0;i<phonesize-7;i++)
        {
            Displayphone.insert(3+i,"*");
        }
    }
     if(sfzhsize!=0&&sfzhsize>7)
      {
          Displaysfzh=socket->message.sfznum.left(3)+socket->message.sfznum.right(4);
          for(int i=0;i<sfzhsize-7;i++)
          {
             Displaysfzh.insert(3+i,"*");
          }
      }
     switch (socket->CardType) {

     case 0:
     case 3:
         ui->frame_10->hide();
         ui->label_czcs->setText(socket->FK_writewithcard.czcs);
         ui->label_czje->setText(socket->FK_writewithcard.czje);
         break;
     case 1:
         ui->frame_9->hide();
         ui->label_bcsjje->setText(socket->LK_writewithcard.gdl);//购电量
         ui->label_wyj->setText(socket->LK_writewithcard.gdcs);//次数
         break;
     default:
         break;
     }
    qDebug()<<Displayname;
    ui->label_ClientName->setText(Displayname);
    ui->label_ClientNum->setText(socket->message.yhdabh);
    ui->label_ContactTelephone_2->setText(Displayphone);
    ui->label_address->setText(socket->message.Address);
}
void WritecardFinished::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label->show();
}
//返回首页
void WritecardFinished::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label->hide();
    emit start(MSEC);
}
void WritecardFinished::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label->hide();
    emit start(MSEC);

}
void WritecardFinished::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        disconnectSlots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
        OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void WritecardFinished::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}

void WritecardFinished::on_pushButton_2_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
void WritecardFinished::on_pushButton_back_2_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void WritecardFinished::on_pushButton_backHomePage_2_clicked()
{
    ui->pushButton_backHomePage_2->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void WritecardFinished::on_pushButton_clicked()
{

}

void WritecardFinished::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    socket->IsPurchase=false;
    disconnectSlots();
    if(OperateFile::ui_lookup==0)
    {
        OperateFile::ui_lookup=new Lookup();
    }
    socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_lookup->init();
}

void WritecardFinished::on_pushButton_4_clicked()
{

}
