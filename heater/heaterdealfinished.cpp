#include "heaterdealfinished.h"
#include "ui_heaterdealfinished.h"
#include"operatefile.h"
#include"first.h"
#include"heaterinputmaount.h"
#include"unionpay.h"
#include"inputpassword.h"
extern ClientSocket *socket;
extern Manager *ui_manager;
Heaterdealfinished::Heaterdealfinished(QStringList &m_httpData, QWidget *parent) :
    httpData(m_httpData),QWidget(parent),
    ui(new Ui::Heaterdealfinished)
{
    ui->setupUi(this);
    ui->frame_5->setObjectName("frame5");
    ui->frame_2->setObjectName("frame2");
    ui->frame_4->setObjectName("frame4");
    ui->frame_3->setObjectName("frame3");
    ui->frame_5->setStyleSheet("QFrame#frame5{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_3->setStyleSheet("QFrame#frame3{background-color:#ebebeb;}");
    ui->frame_4->setStyleSheet("QFrame#frame4{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->pushButton_Print->setEnabled(true);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket,SIGNAL(readyRead()),this,SLOT(HeaterDealfinished_GetData()),Qt::QueuedConnection);
    startTimer();
    display();
    //socket->DataBase.insertData(socket->m_heatersearchmsg.fwbh,socket->m_heatersearchmsg.yhmc,socket->OutData.strMemo,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),ui->label_jfje->text(),"热力","成功","成功","");
}
Heaterdealfinished::~Heaterdealfinished()
{
    delete ui;
}
void Heaterdealfinished::HeaterDealfinished_GetData()
{
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        //格式错误
        return;
    }
    if(list.at(1)=="getstate")
    {
        if(list.at(2)=="ok"||list.at(3)=="1003")
        {
            QString PrintStr;
            QString CurrentTime;
            QString yhmc;
            QString gsrs;
            gsrs.sprintf("%-14s",socket->m_heatersearchmsg.gsrs.toLocal8Bit().data());
            yhmc.sprintf("%-13s",QString(socket->m_heatersearchmsg.yhmc).toLocal8Bit().data());
            CurrentTime=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
            //PrintStr=QString("Network&9008&1&Printer_Print_      新疆锦龙电力自助缴费终端热力交款收据\r\n----------------------------------------------\r\n房屋编号：%1\r\n  门牌号：%2 %3\r\n用户名称：%4\r\n供暖面积：%5\r\n供暖单价：%6\r\n过水热数：%7\r\n实交采暖费：%8\r\n实交违约金：%9\r\n缴费金额：%10\r\n缴费日期：%11\r\n  流水号：%12\r\n客服电话：%13\r\n----------------------------------------------\r\n\r\n\r\n\r\n\r\n").arg(socket->m_heatersearchmsg.fwbh).arg(socket->m_heatersearchmsg.xqmc).arg(socket->m_heatersearchmsg.room_no).arg(socket->m_heatersearchmsg.yhmc).arg(socket->m_heatersearchmsg.srmj).arg(socket->m_heatersearchmsg.gnjg).arg(socket->m_heatersearchmsg.gsrs).arg(QString(httpData.at(2))).arg(QString(httpData.at(4))).arg(QString(httpData.at(1))).arg(CurrentTime).arg(socket->OutData.strMemo).arg("3295598");
            socket->sendMsg(PrintStr);
        }
        else
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法打印凭条");
        }
    }
    if(list.at(1)=="print")
    {
        if(list.at(2)!="ok")
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法打印凭条");
        }
    }
}
void Heaterdealfinished::display()
{
    if(httpData.size()<9)
    {
        return;
    }
    int namesize=socket->m_heatersearchmsg.yhmc.size();
   // int sfzhsiae=socket->m_heatersearchmsg.zjhm.size();
    QString Displayname=socket->m_heatersearchmsg.yhmc;

    if(namesize!=0)
    {
         Displayname=Displayname.replace(0,1,"*");
    }
    ui->label_khmc->setText(Displayname);
    ui->label_fwbh->setText(socket->m_heatersearchmsg.fwbh);
    ui->label_jfje->setText(httpData.at(1));
    ui->label_sjcnf->setText(httpData.at(2));
    ui->label_sjsrf->setText(httpData.at(3));
    ui->label_sjwyj->setText(httpData.at(4));
    ui->label_qjzcnf->setText(httpData.at(5));
    ui->label_qjwyj->setText(httpData.at(6));
    ui->label_qjcnf->setText(httpData.at(7));
    ui->label_qjsrf->setText(httpData.at(8));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_qjzcnf->setPalette(pe);
    ui->label_qjsrf->setPalette(pe);
    ui->label_qjcnf->setPalette(pe);
    ui->label_qjwyj->setPalette(pe);
}
void Heaterdealfinished::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Heaterdealfinished::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=40;
    ui->lcdNumber->display(40);
    ui->frame_lcd->hide();
    emit updateDispaly();
}
void Heaterdealfinished::startTimer()
{
    m_currenttime=40;
    ui->lcdNumber->display(40);
    emit updateDispaly();
}
void Heaterdealfinished::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {
        emit stop();
        disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
        disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
        disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
        disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
        disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
        OperateFile::ui_heaterinputamount->close();
        OperateFile::ui_inputpassword->close();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
        OperateFile::ui_homepage->ejectCard();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}

void Heaterdealfinished::on_pushButton_ConfirmPurchase_4_clicked()
{
    ui->pushButton_ConfirmPurchase_4->setEnabled(false);
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    OperateFile::ui_heaterinputamount->close();
    OperateFile::ui_inputpassword->close();
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);
}

void Heaterdealfinished::on_pushButton_back_clicked()
{
    ui->pushButton_back->setEnabled(false);
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    OperateFile::ui_heaterinputamount->close();
    OperateFile::ui_inputpassword->close();
    OperateFile::ui_homepage->ejectCard();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,WAIYE);

}
void Heaterdealfinished::on_pushButton_Print_clicked()
{
    ui->pushButton_Print->setEnabled(false);
    socket->sendMsg("Network&9008&1&Printer_GetState");
}
