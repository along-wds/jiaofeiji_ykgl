#include "inputpassword.h"
#include "ui_inputpassword.h"
#include"inputamount.h"
#include"unionpay.h"
#include"msgconfirm.h"
#include"readcard.h"
#include"first.h"
#include"heater/heaterinputmaount.h"
#include"manager.h"
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

inputpassword::inputpassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputpassword)
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("text-align: center;color:black;background-color:white;");
    ui->frame_5->setObjectName("frame5");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");

    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_03.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_03.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                  "QPushButton:disabled{border-image: url(:/image/picture/qietu/切图用+_05.png);}");

    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_09.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_09.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_07.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_07.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
     //调色板
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit->setText("");
    m_waitTimeout=false;
    /*密码键盘定时器方法测试*/
    timer2=new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(OpenkeyBoard()));
}
void inputpassword::init()
{
   /* if(socket->Unionpaychannel==true)
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
    socket->IscancelDeal=false;
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
    ui->pushButton_purchase->setEnabled(false);
    ui->pushButton_backhomepage->setEnabled(true);
}
void inputpassword::beginInput()
{
   if(0==openkeyboard())
   {
        timer2->setInterval(20);
        timer2->start();
   }
   else
   {
       OperateFile::tracelog("密码键盘打开失败");
       disconnectSlots();
       socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
       OperateFile::ui_homepage->dealDeviceBroken(PINERROR);
   }
}
inputpassword::~inputpassword()
{
    delete ui;
}
void inputpassword::OpenkeyBoard()
{
        timer2->stop();
        key_out=inputpwd();
        switch (key_out)
        {
        case 0x02:
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"密码输入超时");
            backFirstPage();
            break;
        case 0x08:
            ui->lineEdit->text().clear();
            ui->lineEdit->setText("");
            timer2->start(20);
            startTimer();
            break;
        case 0x2A:     //输入字符
           ui->lineEdit->setText(ui->lineEdit->text()+"●");
           timer2->start(20);
           startTimer();
            break;
        case 0x1B:     //取消交易
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"正在取消……");
            if(closekeyboard()!=0)
                OperateFile::tracelog("close pin error");
            backFirstPage();
            break;
        case 0x0D:     //确认按钮
            CheckPwd();
            ui->lineEdit->clear();
            break;
        case 0xFF:      //无输入
            timer2->start(20);
            break;
        default:
            break;
        }
}
void inputpassword::CheckPwd()
{
       OperateFile::tracelog("***********begin pin check****************");
       QElapsedTimer t;
       t.start();
       while(t.elapsed()<500)
           QCoreApplication::processEvents();
       int ret=clearpwd();
        if(ret==0)
        {

            if(socket->IscancelDeal==false)
            {
                emit stop();
                disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
                disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
                disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
                disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
                disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
                if(socket->Unionpaychannel==true)
                {
                    OperateFile::ui_inputamount=new inputamount;
                    OperateFile::ui_inputamount->setAttribute(Qt::WA_DeleteOnClose);
                    socket->effect->begin(this, OperateFile::ui_inputamount,LEFTTORIGHT,NONE,HIDE);
                    OperateFile::ui_inputamount->init();

                }
                else if(socket->Unionpaychannel==false)
                {
                  OperateFile::ui_heaterinputamount=new Heaterinputmaount;
                  OperateFile::ui_heaterinputamount->setAttribute(Qt::WA_DeleteOnClose);

                  socket->effect->begin(this, OperateFile::ui_heaterinputamount,LEFTTORIGHT,NONE,HIDE);
                  OperateFile::ui_heaterinputamount->init();
                }
            }
            // 撤销交易时输入密码
            else
            {
                cancelDeal();
            }
        }
        else
        {
            OperateFile::tracelog("close pinkeyboard:ret="+QString::number(ret));
            disconnectSlots();
            OperateFile::ui_homepage->dealDeviceBroken(PINERROR);
        }
}
void inputpassword::on_pushButton_backhomepage_clicked()
{
     backFirstPage();
}
void inputpassword::backFirstPage()
{
    ui->pushButton_backhomepage->setEnabled(false);
    timer2->stop();
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    if(closekeyboard()!=0)
        OperateFile::tracelog("close pin error");
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();

}
void inputpassword::startTimer()
{
    m_currenttime=TIMEOUT;
    ui->lcdNumber->display(TIMEOUT);
    emit updateDispaly();
}
void inputpassword::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->display(TIMEOUT);
}
void inputpassword::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=TIMEOUT;
   ui->lcdNumber->display(TIMEOUT);
   emit updateDispaly();
}
void inputpassword::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        disconnectSlots();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void inputpassword::cancelDeal()
{
    /*char Memo[1024];
    char strResp[1164]="";
    memset(Memo,1,sizeof(Memo));
    QString data = QDateTime::currentDateTime().toString("yyyyMMdd");

    QString m_strReq=QString(COUNTERID)+QString(OPERID)+QString(CHEXIAO)+socket->amount_str+QString(socket->OutData.strTrace)
                     +data+QString(OLDREF)+QString(OLDAUTH)+QString(OLDBATCH)+QString(Memo)+QString(LRC);
    OperateFile::tracelog(m_strReq);
    QByteArray ba=m_strReq.toLatin1();

    transcard(ba.data(),strResp);  //交易函数

    QString m_strResp=QString(strResp);
    OperateFile::tracelog(QString::fromLocal8Bit(strResp));
    socket->OutData.strRespcode=m_strResp.mid(0,2);
    OperateFile::tracelog(socket->OutData.strRespcode);
    socket->OutData.strRespInfo=m_strResp.mid(2,40);
    socket->OutData.strCardNo=m_strResp.mid(42,20);
    socket->OutData.strAmount=m_strResp.mid(62,12);
    socket->OutData.strTrace=m_strResp.mid(74,6);
    socket->OutData.strBatch=m_strResp.mid(80,6);
    socket->OutData.strTransDta=m_strResp.mid(86,4);
    socket->OutData.strTransTime=m_strResp.mid(90,6);
    socket->OutData.strRef=m_strResp.mid(96,12);
    socket->OutData.strAuth=m_strResp.mid(108,6);
    socket->OutData.strMid=m_strResp.mid(114,15);
    socket->OutData.strTid=m_strResp.mid(129,8);
    socket->OutData.strMemo=m_strResp.mid(137,1024);
    socket->OutData.strLrc=m_strResp.mid(1161,3);
    OperateFile::tracelog("cancelpayreturncode:"+socket->OutData.strRespcode);
    if(socket->OutData.strRespcode=="00")
    {
        //撤销交易成功
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"交易已取消");
        disconnectSlots();
        OperateFile::ui_homepage->ejectCard();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);

    }
    else if(socket->OutData.strRespcode=="55")
    {
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"密码错误，请重新输入");
        ui->lineEdit->setText("");
        this->beginInput();
    }
    else
    {
        QString string;
        OperateFile::readiniFile("rsp.ini",QString("RESPCODE/")+socket->OutData.strRespcode,string);
        disconnectSlots();
        OperateFile::tracelog(QString(socket->InData.number)+":"+string);
        messagebox->display(this,"交易取消失败，请联系柜台");
        OperateFile::ui_homepage->ejectCard();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);

    }*/
}
void inputpassword::disconnectSlots()
{
    emit stop();
    timer2->stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}
void inputpassword::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    if(closekeyboard()!=0)
        OperateFile::tracelog("close pin error");
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void inputpassword::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots();
    socket->IsPurchase=true;
    socket->effect->begin(this, OperateFile::ui_readcard,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void inputpassword::on_pushButton_search_clicked()
{
    ui->pushButton_search->setEnabled(false);
    disconnectSlots();
    disconnectSlots();
    if(closekeyboard()!=0)
        OperateFile::tracelog("close pin error");
    OperateFile::ui_homepage->ejectCard();
    if(OperateFile::ui_lookup==0)
    {
        OperateFile::ui_lookup=new Lookup();
    }
    socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_lookup->init();

}

void inputpassword::on_pushButton_public_clicked()
{

}
