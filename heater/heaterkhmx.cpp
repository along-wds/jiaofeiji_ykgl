#include "heaterkhmx.h"
#include "ui_heaterkhmx.h"
#include"first.h"
extern Manager *ui_manager;
extern ClientSocket *socket;
Heaterkhmx::Heaterkhmx(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Heaterkhmx)
{
    ui->setupUi(this);
    ui->frame->setObjectName("frame1");
    ui->frame_2->setObjectName("frame2");
    ui->frame_3->setObjectName("frame3");
    ui->frame_4->setObjectName("frame4");
    ui->frame->setStyleSheet("QFrame#frame1{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border:2px groove gray;border-radius:10px;background-color:white;}");
    ui->frame_3->setStyleSheet("QFrame#frame3{background-color:#ebebeb;}");
    ui->frame_4->setStyleSheet("QFrame#frame4{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
     //调色板
   // QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
  //  lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
   // ui->lcdNumber->setPalette(lcdpat);
}
void Heaterkhmx::init()
{
     disPlay();
     m_currenttime=SEC;
     ui->lcdNumber->display(SEC);
     ui->frame_lcd->hide();
     connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
     connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
     connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
     connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
     connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
     //connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
     startTimer();
}
Heaterkhmx::~Heaterkhmx()
{
    delete ui;
}
void Heaterkhmx::replyFinished(QNetworkReply * reply)
{
    //qDebug()<<"khmx_httprecive";

    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         //qDebug()<<QString::fromLocal8Bit(m_receivedata);
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
         //收到客户明细信息
        row=m_HttpData.size();
        QString m_string=m_HttpData.at(0);
        if(m_string.size()<=3)
        {
            //没有记录
        }
        if(m_string.left(2)=="ok")
        {

        }

      }
}

void Heaterkhmx::on_pushButton_backfirst_clicked()
{
    ui->pushButton_backfirst->setEnabled(false);
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);

}
void Heaterkhmx::disPlay()
{
    int namesize=socket->m_heatersearchmsg.yhmc.size();
    int phonesize=socket->m_heatersearchmsg.phone.size();
    int sfzhsiae=socket->m_heatersearchmsg.zjhm.size();
    QString Displayname;
    QString Displayphone;
    QString Displaysfzh;
    QString title;
    if(ui->groupBox_2->title().size()>10)
    ui->groupBox_2->setTitle(ui->groupBox_2->title().remove(5,9));
    title=QString(ui->groupBox_2->title()).insert(5,socket->m_heatersearchmsg.fxzq);
    ui->groupBox_2->setTitle(title);
    if(namesize!=0)
    {
        Displayname=socket->m_heatersearchmsg.yhmc.replace(0,1,"*");
    }
    if(phonesize!=0)
    {
        Displayphone=socket->m_heatersearchmsg.phone.replace(3,4,"*");

    }
     if(sfzhsiae!=0)
      {
          Displaysfzh=socket->m_heatersearchmsg.zjhm.left(3)+socket->m_heatersearchmsg.zjhm.right(4);
          for(int i=0;i<sfzhsiae-7;i++)
          {
              Displaysfzh.insert(3+i,"*");
          }
      }
    ui->label_yhmc->setText(Displayname);
    ui->label_grzmc->setText(socket->m_heatersearchmsg.grzmc);
    ui->label_qjcnf->setText(socket->m_heatersearchmsg.qjcnf);
    ui->label_qjwyj->setText(socket->m_heatersearchmsg.qjwyj);
    ui->label_qjsrf->setText(socket->m_heatersearchmsg.qjsrf);
    ui->label_qjzcnf->setText(socket->m_heatersearchmsg.qjzcnf);
    ui->label_yhdz->setText(socket->m_heatersearchmsg.fwdz);
    ui->label_gsrdj->setText(socket->m_heatersearchmsg.gsrjg);
    ui->label_gndj->setText(socket->m_heatersearchmsg.gnjg);
    ui->label_gsrs->setText(socket->m_heatersearchmsg.gsrs);
    ui->label_xqmc->setText(socket->m_heatersearchmsg.xqmc);
    ui->label_phone->setText(Displayphone);
    ui->label_gnmj->setText(socket->m_heatersearchmsg.srmj);
    ui->label_sfzh->setText(Displaysfzh);

    ui->label_room_no->setText(socket->m_heatersearchmsg.room_no);
    ui->label_fwbh->setText(socket->m_heatersearchmsg.fwbh);
    ui->label_fwlx->setText(socket->m_heatersearchmsg.yhlxmc);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_qjzcnf->setPalette(pe);
    ui->label_qjsrf->setPalette(pe);
    ui->label_qjcnf->setPalette(pe);
    ui->label_qjwyj->setPalette(pe);
}
void Heaterkhmx::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Heaterkhmx::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);

}
void Heaterkhmx::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);
}
void Heaterkhmx::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {
        //qDebug()<<"unionpaytimeout";
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

void Heaterkhmx::on_pushButton_ConfirmPurchase_4_clicked()
{
    ui->pushButton_ConfirmPurchase_4->setEnabled(false);
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);


}
