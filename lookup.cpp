#include "lookup.h"
#include "ui_lookup.h"
#include"readcard.h"
#include"khmx.h"
#include"ydmx.h"
#include"homepage.h"
#include"first.h"
#include"khxx.h"
Lookup::Lookup(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Lookup)
{
    ui->setupUi(this);
    ui->pushButton_yhmx->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/15.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/picture/qietu/15+.png);}");
    ui->pushButton_gdmx->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/16.png);}"
                                            "QPushButton:pressed{border-image:url(:/image/picture/qietu/16+.png);}");
    ui->pushButton_gdmx_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/17.png);}"
                                            "QPushButton:pressed{border-image: url(:/image/picture/qietu/17+.png);}");
    ui->frame_5->setObjectName("frame5");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->pushButton_backhomepage->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_10->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_11->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search+.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}"
                                     "QPushButton:disabled{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_12->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);

}
Lookup::~Lookup()
{
    delete ui;
}
void Lookup::on_pushButton_yhmx_clicked()
{
    ui->pushButton_gdmx->setEnabled(false);
    ui->pushButton_gdmx_2->setEnabled(false);
    ui->pushButton_yhmx->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    disconnectSlots();
    KHxx *form_khmx=new KHxx;
    form_khmx->setAttribute(Qt::WA_DeleteOnClose);
    connect(form_khmx,SIGNAL(back_Lookup()),this,SLOT(Lookup_reshow()));
    socket->effect->begin(this,form_khmx,LEFTTORIGHT,NONE,HIDE);
}
void Lookup::init()
{
    ui->pushButton_gdmx->setEnabled(true);
    ui->pushButton_gdmx_2->setEnabled(true);
    ui->pushButton_yhmx->setEnabled(true);
    ui->pushButton_backhomepage->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(true);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
}
void Lookup::on_pushButton_gdmx_clicked()
{
    ui->pushButton_gdmx->setEnabled(false);
    ui->pushButton_gdmx_2->setEnabled(false);
    ui->pushButton_yhmx->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    disconnectSlots();
    KHmx *form_khmx=new KHmx();
    form_khmx->setAttribute(Qt::WA_DeleteOnClose);
    connect(form_khmx,SIGNAL(back_Lookup()),this,SLOT(Lookup_reshow()));
    socket->effect->begin(this,form_khmx,LEFTTORIGHT,NONE,HIDE);
}
void Lookup::on_pushButton_gdmx_2_clicked()
{
    ui->pushButton_gdmx->setEnabled(false);
    ui->pushButton_gdmx_2->setEnabled(false);
    ui->pushButton_yhmx->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    disconnectSlots();
    Ydmx *form_khmx=new Ydmx();
    form_khmx->setAttribute(Qt::WA_DeleteOnClose);
    connect(form_khmx,SIGNAL(back_Lookup()),this,SLOT(Lookup_reshow()));
    socket->effect->begin(this,form_khmx,LEFTTORIGHT,NONE,HIDE);
}

void Lookup::on_pushButton_back_clicked()
{
   disconnectSlots();
   socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
   OperateFile::ui_homepage->init();
}
/*********底部-首页********/
void Lookup::on_pushButton_backhomepage_clicked()
{
    ui->pushButton_backhomepage->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}
void Lookup::Lookup_reshow()
{
    this->show();

}
void Lookup::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_12->show();
}
void Lookup::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->lcdNumber->hide();
   ui->label_12->hide();
   emit start(MSEC);
}
void Lookup::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label_12->hide();
    emit start(MSEC);
}
void Lookup::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {

       disconnectSlots();
       socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE);
       OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void Lookup::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));

}
/************缴费购电***************/
void Lookup::on_pushButton_10_clicked()
{
    ui->pushButton_10->setEnabled(false);
    socket->IsPurchase=true;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_readcard->init();
}
/**************信息查询*******************/
void Lookup::on_pushButton_11_clicked()
{
    ui->pushButton_11->setEnabled(false);
    socket->IsPurchase=false;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_readcard->init();
}

void Lookup::on_pushButton_12_clicked()
{

}
