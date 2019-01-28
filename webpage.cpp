#include "webpage.h"
#include "ui_webpage.h"
#include "homepage.h"
WebPage::WebPage(QUrl m_url, QWidget *parent) :
    QWidget(parent),sUrl(m_url),
    ui(new Ui::WebPage)
{
    ui->setupUi(this);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    //调色板
    QPalette lcdpat=ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
    //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->axWidget->setProperty("DisplayScrollBars",true); // 显示滚动条
    loadNavigate();

}
void WebPage::init()
{
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()),ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    startTimer();
}
void WebPage::loadNavigate()
{

  ui->axWidget->dynamicCall("Navigate(const QString&)",sUrl);
}
WebPage::~WebPage()
{
    qDebug()<<"delete webpage";
    delete ui;
}
void WebPage::on_pushButton_back_homepage_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
void WebPage::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->label_4->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void WebPage::startTimer()
{
   m_currenttime=SEC;
   ui->lcdNumber->display(SEC);
   ui->label_4->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void WebPage::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_4->show();
}
void WebPage::setLcdnum()
{
   if(ui->lcdNumber->value()==0)
   {

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
void WebPage::disconnectSlots()
{
   emit stop();
   disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
   disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
   disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
   disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
   disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));

}
