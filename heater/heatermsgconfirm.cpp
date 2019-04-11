#include "heatermsgconfirm.h"
#include "ui_heatermsgconfirm.h"
#include"first.h"
#include"unionpay.h"
#include"heaterinputmaount.h"
#include"readnum.h"
extern ClientSocket *socket;
extern Manager *ui_manager;
Heatermsgconfirm::Heatermsgconfirm(QWidget *parent) :
  CommonWidget(0,0,parent),
    ui(new Ui::Heatermsgconfirm)
{
    ui->setupUi(this);
    ui->frame->setObjectName("frame1");
    ui->frame_3->setObjectName("frame3");
    ui->frame_4->setObjectName("frame4");
    ui->frame_5->setObjectName("frame5");
    ui->frame->setStyleSheet("QFrame#frame1{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_3->setStyleSheet("QFrame#frame3{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_4->setStyleSheet("QFrame#frame4{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    ui->frame_5->setStyleSheet("QFrame#frame5{background-color:#ebebeb;}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
}
Heatermsgconfirm::~Heatermsgconfirm()
{
    delete ui;
}
void Heatermsgconfirm::init()
{
    ui->pushButton_ConfirmPurchase->setEnabled(true);
    if(ui->groupBox_2->title().size()>10)
    ui->groupBox_2->setTitle(ui->groupBox_2->title().remove(5,9));
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->frame_lcd->hide();
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
    disPlay();
}
void Heatermsgconfirm::on_pushButton_back_clicked()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    socket->effect->begin(this, OperateFile::ui_readnum,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_readnum->init();
}
void Heatermsgconfirm::on_pushButton_ConfirmPurchase_clicked()
{
    ui->pushButton_ConfirmPurchase->setEnabled(false);
    if(fabs(socket->m_heatersearchmsg.qjzcnf.toDouble())<0.00001)
    {
        messagebox->display(this,"未查询到欠费信息，无法缴费");
    }
    QString string;
    OperateFile::readiniFile("heater.ini","interface/CHECK",string);
    socket->HttpSend(QUrl(string.arg(socket->m_heatersearchmsg.fwbh).arg(socket->m_heatersearchmsg.fxzq).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLatin1()))));
    OperateFile::tracelog(QString("Send jsp:\t")+QString(string.arg(socket->m_heatersearchmsg.fwbh).arg(socket->m_heatersearchmsg.fxzq).arg(socket->LoginMessage.rybh).arg(QString(socket->LoginMessage.rymc.toLatin1()))));
}
void Heatermsgconfirm::replyFinished(QNetworkReply *reply)
{
    //qDebug()<<"heatermsgconfirm_httprecive";
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[1024];
         memset(m_receivedata,0,1024);
         reply->read(m_receivedata,1024);
         //qDebug()<<QString::fromLocal8Bit(m_receivedata);
         QString recvstr=QString::fromLocal8Bit(m_receivedata);
        if(recvstr=="ok@0")
        {
          socket->Unionpaychannel=false;
          disconnectSlots();

          if(OperateFile::ui_unionpay==0)
         {
            OperateFile::ui_unionpay=new UnionPay;
            connect(OperateFile::ui_unionpay,SIGNAL(back_heatermsgconfirm()),this,SLOT(heatermsgconfirm_reshow()),Qt::UniqueConnection);
         }
        socket->effect->begin(this, OperateFile::ui_unionpay,LEFTTORIGHT,NONE,HIDE);
        OperateFile::ui_unionpay->init();
        OperateFile::ui_unionpay->registtoUnion();
        }
       else
        {
            disconnectSlots();
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"减免过水热失败，请到柜台处理！");
            socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
        }
    }
    else
    {
        //服务器拒绝访问
        OperateFile::tracelog("in heatermsgconfirm: server refuse connect");
        disconnectSlots();
        messagebox->displayWithoutButton(this,"设备故障");
    }
    reply->deleteLater();
 }
void Heatermsgconfirm::disPlay()
{
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    /*QPalette lcdpat;
    lcdpat.setColor(QPalette::ButtonText,Qt::white);
     //设置当前窗口的调色板
    ui->pushButton_ConfirmPurchase->setPalette(lcdpat);*/

    int namesize=socket->m_heatersearchmsg.yhmc.size();

    QString Displayname=socket->m_heatersearchmsg.yhmc;

   QString title;
   title=QString(ui->groupBox_2->title()).insert(5,socket->m_heatersearchmsg.fxzq);
   ui->groupBox_2->setTitle(title);
   if(namesize!=0)
   {
        Displayname=Displayname.replace(0,1,"*");
   }

    ui->label_yhmc->setText(Displayname);
    ui->label_qjcnf->setText(socket->m_heatersearchmsg.qjcnf);
    ui->label_qjwyj->setText(socket->m_heatersearchmsg.qjwyj);
    ui->label_qjsrf->setText(socket->m_heatersearchmsg.qjsrf);
    ui->label_qjzcnf->setText(socket->m_heatersearchmsg.qjzcnf);
    ui->label_yhdz->setText(socket->m_heatersearchmsg.fwdz);
    ui->label_gndj->setText(socket->m_heatersearchmsg.gnjg);
    ui->label_gsrs->setText(socket->m_heatersearchmsg.gsrs);
    ui->label_xqmc->setText(socket->m_heatersearchmsg.xqmc);
    ui->label_room_no->setText(socket->m_heatersearchmsg.room_no);
    ui->label_gnmj->setText(socket->m_heatersearchmsg.srmj);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_qjzcnf->setPalette(pe);
    ui->label_qjsrf->setPalette(pe);
    ui->label_qjcnf->setPalette(pe);
    ui->label_qjwyj->setPalette(pe);
}
void Heatermsgconfirm::heatermsgconfirm_reshow()
{
    init();
}
void Heatermsgconfirm::on_pushButton_backfirst_clicked()
{

}
void Heatermsgconfirm::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Heatermsgconfirm::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);

}
void Heatermsgconfirm::startTimer()
{
    emit start(MSEC);
}
void Heatermsgconfirm::setLcdnum()
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
void Heatermsgconfirm::on_pushButton_clicked()
{

    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_readnum,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_readnum->init();
}
void Heatermsgconfirm::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
}
