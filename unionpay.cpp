#include "unionpay.h"
#include "ui_unionpay.h"
#include"usermsg.h"
#include"operatefile.h"
#include<QLibrary>
#include<QMessageBox>
#include"msgconfirm.h"
#include"readcard.h"
#include"first.h"
#include"manager.h"
#include"heater/heaterinputmaount.h"
#include"lookup.h"
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
/*打开读卡器→检测读卡器是否有卡→读卡→关闭读卡器*/
UnionPay::UnionPay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UnionPay)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    ui->frame_5->setObjectName("frame5");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");

    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:disabled{border-image: url(:/image/picture/qietu/purchase.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    connect(this,SIGNAL(initwithUnion()),socket->unionThread,SLOT(initwithUnion()),Qt::QueuedConnection);
    connect(socket->unionThread,SIGNAL(initFinished(int)),this,SLOT(getResp(int)),Qt::QueuedConnection);

}
UnionPay::~UnionPay()
{
    delete ui;
}
void UnionPay::init()
{
    ui->pushButton_back->setEnabled(false);
    ui->pushButton_home->setEnabled(true);
    ui->pushButton_public->setEnabled(true);
    ui->pushButton_purchase->setEnabled(false);
    ui->pushButton_search->setEnabled(true);
    /*if(socket->Unionpaychannel==true)
    {
        ui->label_2->setText("缴费购电");
        ui->label_2->setStyleSheet("color:#0e357f;font-size:25px;font-family: \"Microsoft YaHei UI\";");
        ui->label_2->setAlignment(Qt::AlignCenter);
    }
    else
    {
        ui->label_2->setText("供热缴费");
        ui->label_2->setStyleSheet("color:#0e357f;font-size:25px;font-family: \"Microsoft YaHei UI\";");
        ui->label_2->setAlignment(Qt::AlignCenter);
    }*/
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
    messagebox->displayWithoutBlock(this,"初始化中……");
}
/*签到*/
void UnionPay::registtoUnion()
{
    emit initwithUnion();
}
void UnionPay::getResp(int ret)
{
    messagebox->closeDialog();
    OperateFile::tracelog("unionpay init return ret:"+QString::number(ret));
    if(ret!=0)
    {
        disconnectSlots();
        QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"银联缴费服务暂不可用，请稍后办理");
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
        OperateFile::ui_homepage->init();
    }
    else
    ReadCard();
}
void UnionPay::on_pushButton_back_clicked()
{
    timer->stop();
    disconnectSlots();
    if(initdev()!=0)
        OperateFile::tracelog("open readcarddev fail");
    int ret=closedev(1);
    if(ret==0)
    {
       OperateFile::tracelog("关闭读卡器");
    }
    else
    {
       OperateFile::tracelog("关闭读卡器");
    }
    if(socket->Unionpaychannel==true)
    {
        socket->effect->begin(this, OperateFile::ui_msgconfirm,RIGHTTOLEFT,NONE,HIDE);
        OperateFile::ui_msgconfirm->init();
    }
    else
    {
        emit back_heatermsgconfirm();
        socket->effect->begin(this, OperateFile::ui_heatermsgconfirm,RIGHTTOLEFT,NONE,HIDE);
    }
}
void UnionPay::ReadCard()
{
    OperateFile::tracelog("*********************readcard timer start*********************");
    ui->pushButton_back->setEnabled(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()),Qt::UniqueConnection);
    timer->setInterval(1000);
    timer->start();
}
void UnionPay::onTimerout()
{
    timer->stop();
    int ret=getstate();
    if(ret<0)
    {

        OperateFile::tracelog("银联检测卡失败："+QString::number(ret));
        disconnectSlots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
        OperateFile::ui_homepage->dealDeviceBroken(READCARDBROKEN);
    }
    else if(ret==2)
    {
        getCardNum();
    }
    else{timer->start(1000);}
}
void UnionPay::getCardNum()
{
    int ret;
    ui->pushButton_back->setEnabled(false);
    ret=read(socket->InData.number);
    qDebug()<<"cardnum:"<<socket->InData.number;
    if(ret<0)
    {
       ui->pushButton_back->setEnabled(true);
       QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
       messagebox->displayWithoutButton(this,"请按正确方向插入您的银行卡");
       if(initdev()!=0)
           OperateFile::tracelog("open readcarddev fail");
       int ret=closedev(1);
       if(ret==0)
       {

             init();
             registtoUnion();
       }
       //弹卡失败
       else{
           OperateFile::tracelog("弹卡失败：ret="+QString::number(ret));
           disconnectSlots();
           socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
           OperateFile::ui_homepage->dealDeviceBroken(READCARDBROKEN);}
    }
    else
    {
        ret=closedev(0);
        disconnectSlots();
        inputpassword *form_inputpassword=new inputpassword();
        OperateFile::ui_inputpassword=form_inputpassword;
        form_inputpassword->setAttribute(Qt::WA_DeleteOnClose);
        connect(OperateFile::ui_inputpassword,SIGNAL(back_unionpay()),this,SLOT(unionPay_reshow()),Qt::UniqueConnection);
        socket->effect->begin(this, OperateFile::ui_inputpassword,LEFTTORIGHT,NONE,HIDE);
        OperateFile::ui_inputpassword->init();
        QElapsedTimer t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();
        OperateFile::ui_inputpassword->beginInput();
    }
}
void UnionPay::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_6->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
/*超时返回*/
void UnionPay::waitTimeout()
{
    ui->label_6->show();
    ui->lcdNumber->show();
    emit updateDispaly();
}
void UnionPay::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_6->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
void UnionPay::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {
        timer->stop();
        OperateFile::ui_homepage->ejectCard();
        disconnectSlots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
        OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void UnionPay::unionPay_reshow()
{
    init();
    registtoUnion();
}
void UnionPay::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}

/************************************/
/*热力、电力umsips调用*/
/************************************/
void UnionPay::loadTolib()
{

    qDebug()<<"enter load";
    socket->lib.unload();
    if(socket->Unionpaychannel==true) {

       OperateFile::tracelog(QString("缴费类型:电力"));
       qDebug()<<"缴费类型:电力";
       socket->lib.setFileName("C:/dianli/umsapi.dll");
       if(socket->lib.load())
       {
           qDebug()<<"load successed";
       }
    }
    else
    {
        OperateFile::tracelog(QString("缴费类型:热力"));
        qDebug()<<"缴费类型:热力";
        socket->lib.setFileName("C:/reli/umsapi.dll");
         if( socket->lib.load())
         {
             qDebug()<<("load 2 successed");
         }
    }

}

void UnionPay::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}

void UnionPay::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots();
    socket->IsPurchase=true;
    socket->effect->begin(this, OperateFile::ui_readcard,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_readcard->init();
}

void UnionPay::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots(); 
    if(OperateFile::ui_lookup==0)
    {
        OperateFile::ui_lookup=new Lookup();
    }
    socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_lookup->init();

}

void UnionPay::on_pushButton_public_clicked()
{

}
