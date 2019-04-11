#include "commonwidget.h"
#include "operatefile.h"
#include "homepage.h"
CommonWidget::CommonWidget(QLCDNumber *lcdnumber, QLabel *label, QWidget *parent) :QWidget(parent),
    Timer_LcdNumber(lcdnumber),Timer_Label(label)
{

}
void CommonWidget::waitTimeout()
{
    emit updateDispaly();
    Timer_LcdNumber->show();
    Timer_Label->show();
}
void CommonWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(Timer_LcdNumber!=0&&Timer_Label!=0)
    {
        Currenttime=60;
        OperateFile::hidePanle();
        Timer_LcdNumber->display(Currenttime);
        Timer_LcdNumber->hide();
        Timer_Label->hide();
        emit start(LSEC);
    }
}
void CommonWidget::startTimer()
{
    connect2Timer();
    Currenttime=SEC;
    Timer_LcdNumber->display(60);
    Timer_LcdNumber->hide();
    Timer_Label->hide();
    emit start(10000);
}
void CommonWidget::setLcdnum()
{
    if(Timer_LcdNumber->value()==0)
    {
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,CLOSE,NEIYE);
        //OperateFile::ui_homepage->init();
    }
    else
    {
        Currenttime--;
        Timer_LcdNumber->display(Currenttime);
    }
}
void CommonWidget::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    //disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

}
void CommonWidget::connect2Timer()
{
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::UniqueConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::UniqueConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::UniqueConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::UniqueConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::UniqueConnection);
    //connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
}

