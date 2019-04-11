#include "purchase.h"
#include "ui_purchase.h"
#include<cashpay.h>
#include"homepage.h"
#include"first.h"
#include"writecard.h"
#include"readcard.h"
#include"operatefile.h"
extern HomePage *homepage;
//First ui_first;
PurChase::PurChase(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::PurChase)
{
    ui->setupUi(this);
     //ui->pushButton_cashPay->setStyleSheet("QPushButton{border-image: url(:/purchase/picture/缴费购电/现金缴费.png);}");
     /*ui->pushButton_unionPay->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/08.png);}"
                                            "QPushButton:pressed{border-image: url(:/image/picture/qietu/08响应.png);}");*/
     ui->pushButton_writeCard->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/09.png);}"
                                             "QPushButton:pressed{border-image: url(:/image/picture/qietu/09响应.png);}");
     ui->frame_2->setObjectName("frame2");
     ui->frame_3->setObjectName("frame3");
     ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/xiadaohang.png);}");
     ui->frame_3->setStyleSheet("QFrame#frame3{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
     ui->pushButton_backHomePage_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
     ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                   "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase.png);}");
     ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                     "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
     ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                     "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
     QPalette lcdpat =ui->lcdNumber->palette();
     /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
     lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
      //设置当前窗口的调色板
     ui->lcdNumber->setPalette(lcdpat);
}

PurChase::~PurChase()
{
    delete ui;
}
void PurChase::init()
{
    ui->pushButton_backHomePage_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    socket->IsPurchase=true;
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_6->hide();
    ui->lcdNumber->hide();
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
}
/*现金缴费购电读卡跳转*/
void PurChase::on_pushButton_cashPay_clicked()
{

    socket->PurchaseType=0;

     if(OperateFile::ui_readcard==0)
     {
        OperateFile::ui_readcard=new readcard();
        connect(OperateFile::ui_readcard,SIGNAL(purchase_back()),this,SLOT(Purchase_reshow()));

     }
        OperateFile::ui_readcard->show();
        this->hide();
}

/*void PurChase::on_pushButton_backHomePage_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}*/
void PurChase::Purchase_reshow()
{
    init();
}
/*银联缴费购电读卡跳转*/
void PurChase::on_pushButton_unionPay_clicked()
{

    socket->PurchaseType=1;
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
        connect(OperateFile::ui_readcard,SIGNAL(purchase_back()),this,SLOT(Purchase_reshow()),Qt::UniqueConnection);

    }
    disconnectSlots();
    //OperateFile::ui_readcard->init();
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
}
/*购电写卡跳转*/
void PurChase::on_pushButton_writeCard_clicked()
{
    if(OperateFile::ui_writecard==0)
    {
        OperateFile::ui_writecard=new Writecard();
        connect( OperateFile::ui_writecard,SIGNAL(purchase_back()),this,SLOT(Purchase_reshow()),Qt::UniqueConnection);

    }
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_writecard,LEFTTORIGHT,NONE,HIDE);
    //OperateFile::ui_writecard->init();
}
//返回首页
void PurChase::on_pushButton_backHomePage_2_clicked()
{
    ui->pushButton_backHomePage_2->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
}

//底部-信息查询
void PurChase::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    disconnectSlots();
    socket->IsPurchase=false;
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    //OperateFile::ui_readcard->init();
}
//公共服务
void PurChase::on_pushButton_3_clicked()
{

}
void PurChase::on_pushButton_backHomePage_3_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    //OperateFile::ui_homepage->init();
}
void PurChase::waitTimeout()
{
    emit updateDispaly();
    ui->label_6->show();
    ui->lcdNumber->show();
}
void PurChase::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->label_6->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void PurChase::startTimer()
{
    emit start(MSEC);
}
void PurChase::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       disconnectSlots();
       socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
    }
    else
    {

        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void PurChase::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}

