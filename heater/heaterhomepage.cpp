#include "heaterhomepage.h"
#include "ui_heaterhomepage.h"
#include"operatefile.h"
#include"first.h"
extern ClientSocket *socket;
extern Manager *ui_manager;
Heaterhomepage::Heaterhomepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Heaterhomepage)
{
    ui->setupUi(this);
    form_readnum=NULL;
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/007.png);}"
                                "QPushButton:pressed{border-image: url(:/image/picture/qietu/007响应.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/004.png);}"
                                         "QPushButton:pressed{border-image: url(:/image/picture/qietu/004响应.png);}");
    ui->pushButton_backfirst->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/返回.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    //调色板
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}
void Heaterhomepage::init()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->frame_lcd->hide();
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
}
Heaterhomepage::~Heaterhomepage()
{
    delete ui;
}

void Heaterhomepage::on_pushButton_backfirst_clicked()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
}

void Heaterhomepage::on_pushButton_search_clicked()
{
    socket->Isheaterpurchase=false;
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    if(OperateFile::ui_readnum==0)
       {
         OperateFile::ui_readnum=new Readnum();
         connect(OperateFile::ui_readnum,SIGNAL(heaterhomgage_back()),this,SLOT(reshow_heaterhomepage()),Qt::UniqueConnection);
       }
    OperateFile::ui_readnum->init();
    socket->effect->begin(this,OperateFile::ui_readnum,LEFTTORIGHT,NONE,HIDE,NEIYE);
}
void Heaterhomepage::on_pushButton_purchase_clicked()
{
    /*if(OperateFile::ui_homepage->Couldpurchase==false)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"银联缴费暂不可用，请到柜台办理");
        return;
    }*/
    socket->Isheaterpurchase=true;
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    if(OperateFile::ui_readnum==0)
       {
         OperateFile::ui_readnum=new Readnum();
         connect(OperateFile::ui_readnum,SIGNAL(heaterhomgage_back()),this,SLOT(reshow_heaterhomepage()),Qt::UniqueConnection);
       }
    OperateFile::ui_readnum->init();
    socket->effect->begin(this,OperateFile::ui_readnum,LEFTTORIGHT,NONE,HIDE,NEIYE);
}
void Heaterhomepage::reshow_heaterhomepage()
{
   init();
}
void Heaterhomepage::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Heaterhomepage::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);
}
void Heaterhomepage::startTimer()
{
    emit start(MSEC);
}
void Heaterhomepage::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {
        emit stop();
        disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
        disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
        disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
        disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
        disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
