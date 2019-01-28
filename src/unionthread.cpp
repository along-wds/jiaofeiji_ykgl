#include "unionthread.h"
#include"operatefile.h"
#include"first.h"
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
Unionthread::Unionthread()
{
    transmintstrResp=new char[128];
    memset(transmintstrResp,0,128);
    timer=NULL;
}
Unionthread::~Unionthread()
{
    //delete []strResp;
    delete []transmintstrResp;

}
void Unionthread::initwithUnion()
{
    OperateFile::tracelog("**********************regist to unionpay*********************");
    int ret=-1;
    char m_retcode[4]={};
    ret=regist(1,m_retcode);
    if(strncmp(m_retcode,"00",2)==0)
       OpenDev();
    else
     {
        OperateFile::tracelog("银联初始化失败，错误码："+QString::number(ret));
        emit initFinished(INITERROR);
     }
}
void Unionthread::OpenDev()
{
    int ret=initdev();
    emit initFinished(ret);
}
void Unionthread::ejectCard()
{
    int ret=0;
    if(closedev(1)!=0)
    {
        OperateFile::tracelog("in Unionthread: Eject Card Fail!");
        ret=1;
    }
    if(ret==0)
    {
        OperateFile::tracelog("in Unionthread: Eject and Close Card successful!");
    }
    //emit ejectCardFinished(ret);
}
/*void Unionthread::checkCard()
{
    if(ejectcard()!=0)
    {
        OperateFile::tracelog("in Unionthread: Eject Card Fail!");
    }
    if(timer==NULL)
    {
        timer=new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(checkTimer_out()));
    }
    timer->setInterval(1000);
    timer->start();
}*/
/*void Unionthread::checkTimer_out()
{
    if(0!=checkcard(&stat_out))
    {
        timer->stop();
        OperateFile::tracelog("银联：检测卡失败：");
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
    }

}*/
void Unionthread::dealwithUnion(QString amount)
{
    amount.sprintf("%012.2f",(round(amount.toDouble(),2)));
    amount.remove(".");
    amount.push_front("0");
    QByteArray ba = amount.toLatin1();
    strcpy(socket->InData.amount,ba.data());
#ifdef DEBUG
    char unionstring[]="000105436742*********8701 0000000122242896000000000010                                898150149000003012273311717023735411011171702                                                                                          ";
   char unionstring2[]="75                                        000000000500密码输入次数超限                        89815014900000301227331                                                                                                                ";
    strcpy(socket->RetData.strResp,unionstring);
    socket->OutData.rescode=QString::fromLocal8Bit(mid(0,2));
    OperateFile::tracelog("rescode:"+socket->OutData.rescode);
    socket->OutData.bank_code=QString(mid(2,4));
    OperateFile::tracelog("bankcode:"+socket->OutData.bank_code);
    socket->OutData.card_no=QString::fromLocal8Bit(mid(6,20));

    socket->OutData.expr=QString(mid(26,4));
    socket->OutData.batch=QString(mid(30,6));
    socket->OutData.trace=QString(mid(36,6));
    OperateFile::tracelog("trace:"+socket->OutData.trace);
    socket->OutData.amount=QString(mid(42,12));
    socket->OutData.resp_chin=QString(mid(54,40));
    OperateFile::tracelog("chin:"+socket->OutData.resp_chin);
    socket->OutData.smchtid=QString(mid(94,15));
    OperateFile::tracelog("smchitid:"+socket->OutData.smchtid);
    socket->OutData.stermid=QString(mid(109,8));
    OperateFile::tracelog("stermid:"+socket->OutData.stermid);
    socket->OutData.reference=QString(mid(117,12));
    socket->OutData.sdate=QString(mid(129,4));
    OperateFile::tracelog("sdate:"+socket->OutData.sdate);
    socket->OutData.stime=QString(mid(133,6));
    socket->OutData.info=QString(mid(139,90));
    emit electricDealFinished(amount);
    return;
#endif
    if(0==deal(&(socket->InData),&(socket->RetData)))
    {
       //银联缴费接口调用
        OperateFile::tracelog("unionpay  dealfinish  rescode="+QString::fromLocal8Bit(socket->RetData.strResp));

    }
    else
    {
        //缴费失败
         OperateFile::tracelog("dealfail");
    }


    socket->OutData.rescode=QString::fromLocal8Bit(mid(0,2));
    socket->OutData.bank_code=QString::fromLocal8Bit(mid(2,4));
    OperateFile::tracelog("bankcode:"+socket->OutData.bank_code);
    socket->OutData.card_no=QString::fromLocal8Bit(mid(6,20));

    socket->OutData.expr=QString::fromLocal8Bit(mid(26,4));
    OperateFile::tracelog("expr:"+socket->OutData.expr);
    socket->OutData.batch=QString::fromLocal8Bit(mid(30,6));
    OperateFile::tracelog("batch:"+socket->OutData.batch);
    socket->OutData.trace=QString::fromLocal8Bit(mid(36,6));
    OperateFile::tracelog("trace:"+socket->OutData.trace);
    socket->OutData.amount=QString::fromLocal8Bit(mid(42,12));
    socket->OutData.resp_chin=QString::fromLocal8Bit(mid(54,40));
    OperateFile::tracelog("resp_chin"+socket->OutData.resp_chin);
    socket->OutData.smchtid=QString::fromLocal8Bit(mid(94,15));
    OperateFile::tracelog("smchtid"+socket->OutData.smchtid);
    socket->OutData.stermid=QString::fromLocal8Bit(mid(109,8));
    OperateFile::tracelog("stermid"+socket->OutData.stermid);
    socket->OutData.reference=QString::fromLocal8Bit(mid(117,12));
    socket->OutData.sdate=QString::fromLocal8Bit(mid(129,4));
    socket->OutData.stime=QString::fromLocal8Bit(mid(133,6));
    socket->OutData.info=QString::fromLocal8Bit(mid(139,90));
    if(socket->Unionpaychannel==true)
        emit electricDealFinished(amount);
    else
        emit heaterDealFinished(amount);
}
char * Unionthread::mid(int m, int n)
{

    memset(transmintstrResp,0,128);

    char *p =socket->RetData.strResp;

    char *q =transmintstrResp ;

    int len = strlen(socket->RetData.strResp);

    if(n>len)
        n = len-m;    /*从第m个到最后*/
    if(m<0)
        m=0;    /*从第一个开始*/
    if(m>len)
        return NULL;
    p += m;
    while(n--)
        *(q++) = *(p++);
    *(q++)='\0';

    OperateFile::tracelog(QString::fromLocal8Bit(transmintstrResp));

    return transmintstrResp;
}
double Unionthread:: round(double d, unsigned p)
{
    if(d==0)
        return 0;
    double n=1;
    for(;p>0;p--)
        n*=10;
    if(d>0.0)
        if(d-(__int64)(d*n)/n>=5)
            return (__int64)(d*n+0.5) / n;
        else
            return (__int64)(d*n+0.5000001) / n;
    else
        if(d-(__int64)(d*n)/n>=5)
            return (__int64)(d*n-0.5) / n;
        else
            return (__int64)(d*n-0.5000001) / n;
}
