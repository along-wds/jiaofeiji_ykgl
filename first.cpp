#include "first.h"
#include "ui_first.h"
#include<QPalette>
#include<QBrush>
#include<QPixmap>
#include<QMessageBox>
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

First::First(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::First)
{
    ui->setupUi(this);
    CheckTimes=0;
    Couldpurchase=true;
    form_homepage=NULL;
    form_heaterhomepage=NULL;
    this->setAutoFillBackground(true);
    QPalette palette;
    this->showFullScreen();
    palette.setBrush(this->backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/底纹.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/001.png);}"
                                "QPushButton:pressed{border-image: url(:/image/picture/qietu/001响应.png);}");

    ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/002.png);}"
                                   "QPushButton:pressed{border-image: url(:/image/picture/qietu/002响应.png);}");
    step1=false;
    step2=false;
    step3=false;
   CheckTimer=new QTimer(this);
   connect(CheckTimer,SIGNAL(timeout()),this,SLOT(Checktimer_out()));
}

First::~First()
{
    delete ui;
}
void First::on_pushButton_2_clicked()
{
   //电力自助缴费界面
   QString serviceState;
   OperateFile::readiniFile("config.ini","SERVICE/electric",serviceState);
   if(serviceState=="1"){
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"暂不支持此服务");
        return;}

   /**********************************/
   //socket->Multmessagebox->Display(this,"仅支持如下样式卡的充值服务");
   /**********************************/
   if(OperateFile::ui_homepage==0)
       {
         OperateFile::ui_homepage=new HomePage;
         connect(OperateFile::ui_homepage,SIGNAL(back_first()),this,SLOT(reshow_first()),Qt::UniqueConnection);
       }
    socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
    OperateFile::ui_homepage->init();
}
void First::reshow_first()
{
   //qDebug()<<"first reshow";
}
void First::on_pushButton_clicked()
{
    //热力服务
    QString serviceState;
    OperateFile::readiniFile("config.ini","SERVICE/heater",serviceState);
    if(serviceState=="1"){
         QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
         messagebox->displayWithoutButton(this,"暂不支持此服务");
         return;}
   if( OperateFile::ui_heaterhomepage==0)
       {

         OperateFile::ui_heaterhomepage=new Heaterhomepage;
         connect( OperateFile::ui_heaterhomepage,SIGNAL(back_first()),this,SLOT(reshow_first()),Qt::UniqueConnection);
       }

  socket->effect->begin(this, OperateFile::ui_heaterhomepage,LEFTTORIGHT,NONE,HIDE,WAIYE);
  OperateFile::ui_heaterhomepage->init();

}
void First::dealDeviceBroken(DeviceError ErrorType)
{
    messagebox->closeDialog();
    int ret=closedev(1);
    if(ret==0)
    {
       closedev(1);
    }
    else
    {
      closedev(1);
    }
    switch (ErrorType) {
    case JSPBROKEN:
           messagebox->displayWithoutButton(this,DEVICEBROKEN);
        break;
    case PINERROR:
    case READCARDBROKEN:
        Couldpurchase=false;
        QTimer::singleShot(10000, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"银联缴费暂不可用，请到柜台办理");
        break;
    default:
        break;
    }

}
void First::ejectCard()
{
    if(closedev(1)!=0)
    {
        OperateFile::tracelog(" Eject Card Fail!");

    }
    OperateFile::tracelog("in Unionthread: Eject and Close Card successful!");
}
void First::checkCard()
{
    int ret=closedev(1);
    if(ret==0)
    {
        closedev(1);
    }
    else
    {
        closedev(1);
    }
    //qDebug()<<"check state";
    CheckTimer->setInterval(1000);
    CheckTimer->start();

}
void First::Checktimer_out()
{
    /*uchar stat_out;
    //OperateFile::tracelog("checkcard"+QString::number(ret));

    if(0!=checkcard(&stat_out))
    {
        CheckTimer->stop();
        OperateFile::tracelog("银联：检测卡失败：");
        messagebox->displayWithoutButton(this,DEVICEBROKEN);
        return;
    }
    else
    {
        switch (stat_out) {
        case 0x34:
            CheckTimes++;
            if(30==CheckTimes)
            {
                CheckTimes=0;
                CheckTimer->stop();
                OperateFile::tracelog("用户未取卡，吞卡");
                cardswallow();
                cardclose();
                OperateFile::tracelog("用户长时间未取走卡，设备吞卡");
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"用户长时间未取走卡，设备吞卡");
            }
            break;
        case 0x35:
            CheckTimer->stop();
            if(0==cardclose())
                OperateFile::tracelog("关闭读卡器0x35");
            break;
        case 0x37:
            ejectcard();
            if(0==cardclose())
                OperateFile::tracelog("关闭读卡器0x37");
            break;
        case 0x38:
            ejectcard();
            if(0==cardclose())
                OperateFile::tracelog("关闭读卡器0x38");
            break;
        default:
            break;
        }
    }*/
}
//组合点击退出
void First::mousePressEvent(QMouseEvent *event)
{
   if(event->pos().rx()<100&&event->pos().ry()<100&&step1==false&&step2==false&&step3==false)
        step1=true;
  else if((event->pos().ry()>672)&&event->pos().rx()>924&&step1==true&&step2==false&&step3==false)
        step2=true;
  else if((event->pos().ry()<100)&&event->pos().rx()>924&&step1==true&&step2==true&&step3==false)
        step3=true;
  else if((event->pos().rx()<100)&&(event->pos().ry()>672)&&step1==true&&step2==true&&step3==true)
        {

            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"程序即将退出");
            qApp->quit();
        }
   else
   {
       step1=false;
       step2=false;
       step3=false;
   }

}
void First::init()
{

}
