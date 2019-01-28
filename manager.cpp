#include "manager.h"
#include<qdebug.h>
#include<QThread>
Manager::Manager()
{
    timer=NULL;
}
Manager::~Manager()
{
    delete timer;
}
void Manager::onTimerout()
{
    timer->stop();
    emit timeout();
}
void Manager::starttimer(long msec)
{
    if(timer==NULL)
    {
        timer=new QTimer;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimerout2()));
        connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()),Qt::UniqueConnection);
        timer->setInterval(msec);
        timer->start();
        //qDebug()<<"timer start";
    }
    else
    {
        timer->stop();
        disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimerout2()));
        connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()),Qt::UniqueConnection);
        timer->setInterval(msec);
        timer->start();
    }
}
void Manager::stoptimer()
{
    timer->stop();
}
void Manager::starttimer2()
{
    if(timer==NULL)
    {
        timer=new QTimer;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
        connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout2()),Qt::UniqueConnection);
        timer->setInterval(1000);
        timer->start();
    }
    else
    {
        //qDebug()<<"starttimer2";
        disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimerout()));
        connect(timer,SIGNAL(timeout()),this,SLOT(onTimerout2()),Qt::UniqueConnection);
        timer->setInterval(1000);
        timer->stop();
        timer->start();
    }
}
void Manager::onTimerout2()
{
    emit timeout2();
}
