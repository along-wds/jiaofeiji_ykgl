#include "login.h"
#include "ui_login.h"
#include<QTimer>
#include<QMessageBox>
#include<QGridLayout>
#include"homepage.h"
#include<QGridLayout>
#include<QLineEdit>
#include<QDesktopWidget>
#include<QTextCodec>
#include<QNetworkInterface>
#include<QSqlDatabase>
#include<qsqlquery.h>
#include<QCryptographicHash>
#include"webpage.h"
Manager *ui_manager;
ClientSocket *socket;
commonDialog *messagebox;
ChioceDialog *choicebox;
Timertask *timertask;
First *OperateFile::ui_first=0;
HomePage* OperateFile::ui_homepage=0;
readcard *OperateFile::ui_readcard=0;
MsgConfirm *OperateFile::ui_msgconfirm=0;
UnionPay  *OperateFile::ui_unionpay=0;
Writecard *OperateFile::ui_writecard=0;
inputpassword* OperateFile::ui_inputpassword=0;
inputamount *OperateFile::ui_inputamount=0;
Lookup *OperateFile::ui_lookup=0;
PurChase *OperateFile::ui_purchase=0;
CashPay *OperateFile::ui_cashpay=0;
ChoiceUser *OperateFile::ui_choiceuser=0;
WebPage *OperateFile::ui_webpage=0;
IdentifyCode * OperateFile::ui_identifycode=0;
/*热力界面*/
 Heaterinputmaount *OperateFile::ui_heaterinputamount=0;
 Heaterdealfinished *OperateFile::ui_heaterdealfinished=0;
 Heaterhomepage *OperateFile::ui_heaterhomepage=0;
 Heatermsgconfirm *OperateFile::ui_heatermsgconfirm=0;
 Heaterlookup *OperateFile::ui_heaterlookup=0;
 Heaterkhmx *OperateFile::ui_heaterkhmx=0;
 Readnum *OperateFile::ui_readnum=0;
/*业扩管理*/
Jmsh1 *OperateFile::ui_jmsh1=0;
Jmsh2 *OperateFile::ui_jmsh2=0;
Jmsh3 *OperateFile::ui_jmsh3=0;
Jmsh4 *OperateFile::ui_jmsh4=0;
Zdbdhomepage *OperateFile::ui_zdbdhomepage=0;
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

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
   /******************************
    * UI 处理
    * ****************************/
   ui->setupUi(this);
   ui->frame->setObjectName("myframe");
   ui->frame->setStyleSheet("QFrame#myframe{border-image: url(:/image/picture/qietu/01.png);}");
   /*QPalette palette1;
   this->showFullScreen();
   palette1.setBrush(this->backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/底纹.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
   this->setPalette(palette1);*/
   QPalette lcdpat;
   lcdpat.setColor(QPalette::ButtonText,Qt::white);
    //ui->pushButton->setPalette(lcdpat);
    ui->lineEdit->setPlaceholderText("请输入用户名");
    ui->lineEdit_2->setPlaceholderText("请输入密码");
    ui->lineEdit_2->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    //ui->lineEdit->setFocus();
    //ui->lineEdit->installEventFilter(this);  //在窗体上为lineEdit1安装过滤器
    //ui->lineEdit_2->installEventFilter(this);  //在窗体上为lineEdit2安装过滤
    ui->pushButton->setShortcut(Qt::Key_Return);
    /**********************
     * new
     * ********************/
    socket=new ClientSocket();
    thread = new QThread;
    timed_thread=new QThread;
    heartbeat_thread=new QThread;
    ui_manager=new Manager;
    timertask=m_task=new Timertask;
    m_heartbeat_task=new HeartbeatTask;
    messagebox=new commonDialog(this);
    choicebox=new ChioceDialog(this);

    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(login_GetData()));
    connect(this,SIGNAL(login_HasData(KIND)),this,SLOT(login_DealData(KIND)));
    connect(timed_thread,SIGNAL(started()),timertask,SLOT(init()));
    connect(heartbeat_thread,SIGNAL(started()),m_heartbeat_task,SLOT(init()));
    /*************************
     *move to thread
    **************************/
    ui_manager->moveToThread(thread);
    socket->unionThread->moveToThread(socket->thread);
    m_task->moveToThread(timed_thread);
    thread->start();
    socket->thread->start();
    timed_thread->start();

    QString UpdateDir;
    QProcess p;
    QList<UpdateFile>list_server;
    OperateFile::readiniFile("config.ini","path/UPDATEDIR",UpdateDir);

    OperateFile::getUpdateFileList(UpdateDir+"update.xml",list_server);
    for(int i=0;i<list_server.size();i++)
    {
        if(list_server.at(i).name.contains("Launch.exe"))
        {
            p.execute("taskkill /im Launch.exe /f");
            p.waitForFinished();
            OperateFile::tracelog(QString("copy file==============%1==>%2").arg(UpdateDir+list_server.at(i).name).arg(list_server.at(i).path+list_server.at(i).name));
            OperateFile::copyFileToPath(UpdateDir+list_server.at(i).name,list_server.at(i).path+list_server.at(i).name,true);

            //QProcess::startDetached("Launch.exe");
            break;
        }

    }
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
         if(address.protocol() == QAbstractSocket::IPv4Protocol)
            socket->LoginMessage.ip=address.toString();
    }
    QString inistring;
    OperateFile::readiniFile("INTERFACE.DATA","interface/PUBLIC",inistring);
    OperateFile::ui_webpage=new WebPage(QUrl(inistring));

}
void login::startWithoutPwd()
{
    //this->close();
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(login_GetData()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_getlist(QNetworkReply*)));
    m_heartbeat_task->LoginMessage=socket->LoginMessage;
    heartbeat_thread->start();
    messagebox->closeDisplay();
    OperateFile::ui_homepage=new HomePage;
    socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}
void login::showKeyBoard()
{
    this->myKeyboard= new widgetKeyBoard(false);
    this->myKeyboard->createKeyboard(); // only create keyboard
    this->myKeyboard->show(this);
    QDesktopWidget *deskdop = QApplication::desktop();

    /*处理多显示器显示分屏问题*/
    QDesktopWidget * deskTop = QApplication::desktop();
    int curMonitor = deskTop->screenNumber( this ); // 参数是一个QWidget*
    QRect rect = deskTop->screenGeometry(curMonitor);
    this->myKeyboard->move((rect.width()-this->myKeyboard->width())/2, (rect.height() -this->myKeyboard->height())-2);
    socket->player->setMedia(QUrl::fromLocalFile("sounds/1.mp3"));
    //socket->player->setVolume(50);
    //socket->player->play();
    //socket->player->stop();
}
login::~login()
{
    //qDebug()<<"delete login";
    delete ui;
}
void login::on_pushButton_clicked()
{
   OperateFile::hidePanle();
   if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty())
    {
       QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
       messagebox->displayWithoutButton(this,"请输入用户名和密码");
       return;
    }
   QString mac_str,temp_str;
   QList<QNetworkInterface> netInterface=QNetworkInterface::allInterfaces();
   foreach(QNetworkInterface ni,netInterface)
   {
          mac_str=ni.hardwareAddress();
          break;
   }
   messagebox->wait(this,"正在登录……");
   mac_str.replace(":","");
   temp_str=mac_str;
   QString string,Tid,union_str,aqyzm;
   OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",Tid);
   OperateFile::readiniFile("INTERFACE.DATA","interface/login",string);
   union_str=temp_str.remove("-")+Tid+"algr"+ui->lineEdit->text();
   QString teststring=QString("abc123");
   QByteArray bytetest=QCryptographicHash::hash(teststring.toLatin1(), QCryptographicHash::Md5);
   QByteArray bytePwdMd5=QCryptographicHash::hash(union_str.toLatin1(), QCryptographicHash::Md5);
   aqyzm.append(bytePwdMd5.toHex());
   qDebug()<<"aqyzm:"<<aqyzm.toUpper()<<"union:"<<union_str<<"test:"<<QString(bytetest.toHex()).toUpper();
   socket->HttpSend(QUrl(QString(string).arg(Tid).arg(ui->lineEdit->text()).arg(mac_str).arg(aqyzm.toUpper()).arg(ui->lineEdit_2->text())));
   OperateFile::tracelog("Send jsp:\t"+QString(string).arg(Tid).arg(ui->lineEdit->text()).arg(mac_str).arg(aqyzm.toUpper()).arg(ui->lineEdit_2->text()));
}
void login::dosomething()
{
    //qDebug()<<"connected";
}
/*Tcp读取数据*/
void login::login_GetData()
{
   QStringList list=socket->ToString().split("_");
   if(list.at(1)=="Network&9008&1&EC")
    {
            Printer_state=list.at(2);
            socket->sendMsg("Network&9019&1&EC_GetState");
    }
   else if(list.at(0)=="Network&9019&1&EC")
    {
        ElectricCard_state=list.at(2);
    }
    
}
/*Tcp处理回复消息*/
void login::login_DealData(KIND a_reply)
{
    switch(a_reply)
    {
       case LOGINSUCCESS:
        break;
    }
}
/*初始化设备，检查硬件状态*/
void login::InitDevice()
{
    /*银联设备查询*/
    /*打印机状态查询*/
   //socket->sendMsg("Network&9008&1&Printer_GetState");
    socket->sendMsg("Network&9008&1&Printer_GetState");
    socket->sendMsg("Network&9019&1&EC_ReadCard");
}
/*http处理消息*/
void login::replyFinished(QNetworkReply* reply)
{
  if(reply->error() == QNetworkReply::NoError)
   {
        char m_receivedata[512];
        memset(m_receivedata,0,512);
        reply->read(m_receivedata,512);
        OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
        QStringList m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
        QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
        if(base_arg.at(0)=="ok")
        {
            if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
            {
                OperateFile::tracelog("error argument");
                return;
            }
           if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LOGIN)
           {
               OperateFile::tracelog("***************************************登录*********************************************");

               int ret=QString(m_HttpData.at(1)).toInt();
               switch (ret)
               {
               case 0:
               case 3:
                   socket->LoginMessage.rybh=m_HttpData.at(2);
                   socket->LoginMessage.rymc=m_HttpData.at(3);
                   socket->LoginMessage.gsbh=m_HttpData.at(4);
                   socket->LoginMessage.fgsbh=m_HttpData.at(5);
                   socket->LoginMessage.yyzbh=m_HttpData.at(6);
                   socket->LoginMessage.sfsjsfjg=m_HttpData.at(7);
                   disconnect(socket,SIGNAL(readyRead()),this,SLOT(login_GetData()));
                   disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                   disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_getlist(QNetworkReply*)));
                   m_heartbeat_task->LoginMessage=socket->LoginMessage;
                   heartbeat_thread->start();
                   OperateFile::ui_homepage=new HomePage();
                   socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,HIDE);
                   OperateFile::ui_homepage->init();
                   messagebox->closeDisplay();
                   //myKeyboard->hide(true);
                   break;                
               case 1:
                   messagebox->closeDisplay();
                   messagebox->display(this,("MAC未匹配"));
                   break;
               case 2:
                   messagebox->closeDisplay();
                   messagebox->display(this,("安全验证码未维护或者不匹配"));
                   break;
               case 31:
                   messagebox->closeDisplay();
                   messagebox->display(this,("登陆账号未维护到人员"));
                   break;
               case 4:
                   messagebox->closeDisplay();
                   messagebox->display(this,("银联终端号未匹配"));
                   break;
               case 5:
                   messagebox->closeDisplay();
                   messagebox->display(this,("人员不匹配表"));
                   break;
               default:
                   break;
               }

           }

        }
        else if(base_arg.at(0)=="error")
        {
            messagebox->closeDisplay();
            messagebox->display(this,("用户名或密码错误"));
        }
     }
     else
     {
          OperateFile::tracelog(QString(NETWORKERROR)+"in login");
          messagebox->closeDisplay();
          messagebox->display(this,tr("网络错误"));
     }
     reply->deleteLater();
}
void login::replyFinished_getlist(QNetworkReply *reply)
{
    //qDebug()<<"begin getlist";
    if(reply->error() == QNetworkReply::NoError)
     {
          char m_receivedata[1024*10];
          memset(m_receivedata,0,10240);
          reply->read(m_receivedata,10240);
          /*对返回串做处理*/
           /*写日志*/
        QString recvstr=QString::fromLocal8Bit(m_receivedata);
        if(recvstr.left(2)=="ok")
        {
            QString recvsrt_temp=recvstr.remove(0,3);
            socket->qy_list= recvsrt_temp.split("!#!");
            foreach (QString string, socket->qy_list)
            {
                QStringList list=string.split("@");
                if(!socket->qybh_qymc_list.contains(list.at(0)))
                {
                   socket->qybh_qymc_list.insert(list.at(0),list.at(1));
                }
                socket->qybh_xqbh_list.insertMulti(list.at(0),list.at(2));

                socket->xqbh_xqmc_list.insert(list.at(2),list.at(3));
            }
            //qDebug()<<QString::fromLocal8Bit(m_receivedata);
            myKeyboard->hide(true);     
            OperateFile::tracelog("获取小区列表成功");
            messagebox->closeDisplay();
            HomePage *form_homepage=new HomePage;
            OperateFile::ui_homepage=form_homepage;
            disconnect(socket,SIGNAL(readyRead()),this,SLOT(login_GetData()));
            disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
            disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_getlist(QNetworkReply*)));
            this->close();
        }
        else if(socket->xiaoqu_list.at(0)=="error")
        {

            OperateFile::tracelog(QString::fromLocal8Bit(m_receivedata));
        }
        else
        {
            OperateFile::tracelog(QString::fromLocal8Bit(m_receivedata));
        }
     }
      else
      {

         OperateFile::tracelog(QString(NETWORKERROR)+"in login_getlist");
         messagebox->display(this,tr("网络错误"));

      }
      reply->deleteLater();
}
bool login::eventFilter(QObject *watched, QEvent *event)
{
     if (watched==ui->lineEdit)         //首先判断控件(这里指 lineEdit1)
     {
          if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
          {

              this->myKeyboard->show(this);
             // QDesktopWidget *deskdop = QApplication::desktop();
              QDesktopWidget * deskTop = QApplication::desktop();
              int curMonitor = deskTop->screenNumber( this ); // 参数是一个QWidget*
              QRect rect = deskTop->screenGeometry(curMonitor);
              this->myKeyboard->move((rect.width()-this->myKeyboard->width())/2, (rect.height() -this->myKeyboard->height())-2);
              ui->lineEdit_2->clearFocus();
              myKeyboard->focusThis(ui->lineEdit);
          }
          else if (event->type()==QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件
          {

          }
     }
     if (watched==ui->lineEdit_2)           //这里来处理 lineEdit2 , 和处理lineEdit1 是一样的
     {
          if (event->type()==QEvent::FocusIn)
         {
            ui->lineEdit->clearFocus();
             myKeyboard->focusThis(ui->lineEdit_2);
            // to move keyboard
          }
         else if (event->type()==QEvent::FocusOut)
         {

         }
     }
 return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框*/
}
//组合点击退出
void login::mousePressEvent(QMouseEvent *event)
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
