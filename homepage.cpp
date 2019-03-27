#include "ui_homepage.h"
#include "homepage.h"
#include"lookup.h"
#include"readcard.h"
#include"first.h"
#include "devlist.h"
#include"msgconfirm.h"
#include"webpage.h"
#include "ykgl/zdbdhomepage.h"
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
HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    form_PurChase=NULL;
    //timer=new QTimer(this);
    ui->label->setStyleSheet("QLabel{color:white}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/005.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/picture/qietu/005.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/004.png);}"
                                         "QPushButton:pressed{border-image: url(:/image/picture/qietu/004.png);}");
    ui->pushButton_writecard->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/003.png);}"
                                            "QPushButton:pressed{border-image: url(:/image/picture/qietu/003.png);}");
    ui->pushButton_zdbd->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/006.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/qietu/006.png);}");
    connect(timertask,SIGNAL(hascard()),this,SLOT(readyReadCard()),Qt::QueuedConnection);
    connect(timertask,SIGNAL(hascardmsg(QString)),this,SLOT(getCardMsg(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(setTaskstate(bool)),timertask,SLOT(setTimerstate(bool)),Qt::QueuedConnection);
    connect(this,SIGNAL(startReadCardTimer()),timertask,SLOT(StartTimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(reCheck()),timertask,SLOT(Timertask_reCheckCard()),Qt::QueuedConnection);
    socket->Isjumpreadcard=false;
    timertask->LoginMessage=socket->LoginMessage;
    QDir dir("advertisement");
    if(dir.exists())
    {
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        int file_count = list.count();
        for(int i=0;i<file_count; i++)
        {
            QFileInfo file_info = list.at(i);
            QString m_filepath = file_info.filePath();
            ui->widget->addImage(m_filepath);
        }
    }
    QString UpdateDir;
    QList<UpdateFile>list_server;
    OperateFile::readiniFile("config.ini","path/UPDATEDIR",UpdateDir);

    OperateFile::getUpdateFileList(UpdateDir+"update.xml",list_server);
    for(int i=0;i<list_server.size();i++)
    {
        if(list_server.at(i).name.contains("Launch.exe"))
        {
            OperateFile::tracelog(QString("copy file==============%1==>%2").arg(UpdateDir+list_server.at(i).name).arg(list_server.at(i).path+list_server.at(i).name));
            OperateFile::copyFileToPath(UpdateDir+list_server.at(i).name,list_server.at(i).path+list_server.at(i).name,true);
            //QProcess::startDetached("Launch.exe");
            break;
        }
    }
    ui->widget->initChangeImageButton();
}
void HomePage::init(int flag)
{
    step1=false;
    step2=false;
    step3=false;
    Couldpurchase=true;
    if(flag==1){
        emit reCheck();}
    QString string;
    OperateFile::readiniFile(SERVERICEINFO,"SERVICE/AutoReadCard",string);
    if(string=="1")
        emit startReadCardTimer();
    ui->pushButton_purchase->setEnabled(true);
    ui->pushButton_search->setEnabled(true);
    ui->pushButton_writecard->setEnabled(true);
    ui->widget->startPlay();
}
HomePage::~HomePage()
{
    delete ui;
}
void HomePage::readyReadCard()
{
   messagebox->displayWithoutBlock(this,"正在读卡，请稍后……");
}
void HomePage::getCardMsg(QString s_msg)
{
     QStringList list=s_msg.split("@");
     if(QString(list.at(0))=="ok")
     {
        socket->Isjumpreadcard=true;
        socket->CardType=timertask->CardType;
        socket->ReadCard_str=timertask->ReadCard_str;
        socket->Card_Basemsg=timertask->Card_Basemsg;
        socket->LK_msg=timertask->LK_msg;
        socket->FK_msg=timertask->FK_msg;
        socket->message=timertask->message;
        if(OperateFile::ui_msgconfirm==0)
        {
           OperateFile::ui_msgconfirm =new MsgConfirm();
        }
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
            //调整金额隐藏
            //您的缴费金额应大于  页面显示
             OperateFile::ui_msgconfirm->hideTzje();
        }
          emit setTaskstate(true);

     }
     else if(QString(list.at(0))=="warn")
     {
        Couldpurchase=false;
        warnMsg=QString(list.at(1));
     }
     else if(QString(list.at(0))=="error")
     {
        messagebox->closeDialog();
        QTimer::singleShot(4000, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,QString(list.at(1)));
        emit setTaskstate(true);
     }
     else if(QString(list.at(0))=="info"&&(ui->pushButton_purchase->isEnabled()&&ui->pushButton_search->isEnabled()&&ui->pushButton_writecard->isEnabled()))
     {
        if(QString(list.at(1))=="changecard")
            socket->Isjumpreadcard=false;
        emit setTaskstate(true);
     }
     messagebox->closeDialog();
}
/*缴费购电跳转*/
void HomePage::on_pushButton_purchase_clicked()
{
   /* if(OperateFile::ui_homepage->Couldpurchase==false)
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"银联缴费暂不可用，请到柜台办理");
        return;
    }*/
    //disconnectSlots();
    //正常读卡页面
    ui->pushButton_purchase->setEnabled(false);
    emit setTaskstate(false);
    socket->IsPurchase=true;
    if(socket->Isjumpreadcard==false)
    {
        if(OperateFile::ui_readcard==0)
        {
            OperateFile::ui_readcard=new readcard();
        }
        OperateFile::ui_readcard->init();
        socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    }
    //直接跳转到信息展示页
    else
    {
        socket->effect->begin(this, OperateFile::ui_msgconfirm,LEFTTORIGHT,NONE,HIDE);
        OperateFile::ui_msgconfirm->init();
    }
}
/*信息查询跳转*/
void HomePage::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    emit setTaskstate(false);
    socket->IsPurchase=false;
    if(socket->Isjumpreadcard==false)
    {
        if(OperateFile::ui_readcard==0)
        {
            OperateFile::ui_readcard=new readcard();
        }
        OperateFile::ui_readcard->init();
        socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,HIDE);
    }
    else
    {
        if(OperateFile::ui_lookup==0)
        {
            OperateFile::ui_lookup=new Lookup();
        }
       socket->effect->begin(this,OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
       OperateFile::ui_lookup->init();
    }

}

/*void HomePage::reshow_HomePage()
{
    this->init();
}*/

/*void HomePage::on_pushButton_backfirst_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
}
void HomePage::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}*/
/*void HomePage::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);
}*/
/*void HomePage::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->frame_lcd->hide();
    emit start(MSEC);
    emit startReadCardTimer();
}
void HomePage::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {
        disconnectSlots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void HomePage::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));

}*/
//组合点击退出
void HomePage::mousePressEvent(QMouseEvent *event)
{
   if(event->pos().rx()<100&&event->pos().ry()<100&&step1==false&&step2==false&&step3==false)
        step1=true;
  else if((event->pos().ry()>672)&&event->pos().rx()>924&&step1==true&&step2==false&&step3==false)
        step2=true;
  else if((event->pos().ry()<100)&&event->pos().rx()>924&&step1==true&&step2==true&&step3==false)
        step3=true;
  else if((event->pos().rx()<100)&&(event->pos().ry()>672)&&step1==true&&step2==true&&step3==true)
        {

            /*QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"程序即将退出");
            qApp->quit();*/
            Devlist *console=new Devlist;
            //socket->effect->begin(this,console,LEFTTORIGHT,NONE,HIDE);
            console->show();
            this->hide();
        }
   else
   {
       step1=false;
       step2=false;
       step3=false;
   }

}
void HomePage::dealDeviceBroken(DeviceError ErrorType)
{
    messagebox->closeDialog();
    if(socket->PurchaseType==1)//银联缴费
    {
        if(initdev()!=0)
            OperateFile::tracelog("open readcarddev fail");
        if(closedev(1)!=0)
        {
            OperateFile::tracelog("in Unionthread: Eject and Close Card fail!");
            QTimer::singleShot(10000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"弹卡失败，请联系工作人员");
        }
    }
    switch (ErrorType) {
    case JSPBROKEN:
           QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"服务暂不可用，请稍后再试");
        break;
    case PINERROR:
    case READCARDBROKEN:
        QTimer::singleShot(6000, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"银联缴费暂不可用，请到柜台办理");
        break;
    default:
        break;
    }
    this->init();

}
void HomePage::ejectCard()
{
    if(initdev()!=0)
        OperateFile::tracelog("open readcarddev fail");
    if(closedev(1)!=0)
    {
        OperateFile::tracelog("in Unionthread: Eject and Close Card fail!");
        QTimer::singleShot(10000, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"弹卡失败，请联系工作人员");
    }
}

void HomePage::on_pushButton_writecard_clicked()
{
    emit setTaskstate(false);
    socket->effect->begin(this, OperateFile::ui_webpage,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_webpage->init();

}

void HomePage::on_pushButton_zdbd_clicked()
{
    messagebox->closeDialog();
    QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
    messagebox->displayWithoutButton(this,"功能建设中……");
    /*if(!OperateFile::ui_zdbdhomepage)
    {
       OperateFile::ui_zdbdhomepage=new Zdbdhomepage();
       qDebug()<<"1111111111111111";
       //OperateFile::ui_jmsh1->setAttribute(Qt::WA_DeleteOnClose);
    }
    socket->effect->begin(this,OperateFile::ui_zdbdhomepage,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_zdbdhomepage->init();*/
}
