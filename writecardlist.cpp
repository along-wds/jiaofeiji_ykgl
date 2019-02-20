#include "writecardlist.h"
#include "ui_writecardlist.h"
#include<QHeaderView>
#include"writecard.h"
#include<QScrollBar>
#include "qradiobutton.h"
#include "readcard.h"
#include"lookup.h"
Writecardlist::Writecardlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Writecardlist)
{
    ui->setupUi(this);
    writetimes=0;
    write_button=false;
    btnGroup=new QButtonGroup(this);
    ui->frame->setObjectName("frame1");
    ui->frame_5->setObjectName("frame5");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->frame->setStyleSheet("QFrame#frame1{background-color:#dfdfdf;border:1px;border-radius:3px;}");
    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 22px;}"
      "border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:{background:gray;}"
      "QScrollBar::add-line:vertical {border: 2px solid grey;height: 18px;image:url(:/image/picture/qietu/xiala.png);subcontrol-position: bottom;subcontrol-origin: margin;background-color:#ebebeb;"
      "QScrollBar::sub-line:vertical {border: 2px solid grey;height: 18px;image:url(:/image/picture/qietu/shangla.png);subcontrol-position: top;subcontrol-origin: margin;background-color:#ebebeb;}"
       "QScrollBar::up-arrow:vertical{width: 3px;height: 3px;}"
      "QScrollBar::down-arrow:vertical {width: 3px;height: 3px;}" );
    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_03.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_03.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_05.png);}"
                                           "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                           "QPushButton:disabled{border-image: url(:/image/picture/qietu/切图用+_05.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_09.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_09.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_07.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_07.png);}");
    ui->tableWidget->setStyleSheet("QTableView::item{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}"
                                    "QTableView::item:selected{color:#00a797;background:white;border-top:1px solid #00a797;border-bottom:1.5px solid #00a797;}");

    connect(socket,SIGNAL(readyRead()),this,SLOT(writecardlist_GetData()),Qt::UniqueConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(changeCheckstate(int,int,int,int)));
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(MouseTrackItem(int)));
    ui->pushButton_purchase->setEnabled(false);
    if(socket->CardType==0||socket->CardType==3)
    {
        QString string;
        OperateFile::readiniFile("INTERFACE.DATA","interface/GETFKBKLB",string);
        socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->fkbkzt_msg.yffid).arg(socket->fkbkzt_msg.djdm).arg(socket->fkbkzt_msg.dbxb).arg(socket->fkbkzt_msg.zhbl).arg(QString(socket->fkbkzt_msg.djmc.toLocal8Bit().toPercentEncoding()))));
        OperateFile::tracelog("Send jsp:\t"+QString(string).arg(socket->FK_msg.yhdabh).arg(socket->fkbkzt_msg.yffid).arg(socket->fkbkzt_msg.djdm).arg(socket->fkbkzt_msg.dbxb).arg(socket->fkbkzt_msg.zhbl).arg(socket->fkbkzt_msg.djmc).toLocal8Bit());
    }
    else if(socket->CardType==1)
    {
        QString string;
        OperateFile::readiniFile("INTERFACE.DATA","interface/GETLKBKLB",string);
        socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.ickid)));
        OperateFile::tracelog("Send jsp:\t"+QString(string).arg(socket->LK_msg.ickid));
    }
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    QPalette widgetpat=ui->tableWidget->palette();
    widgetpat.setBrush(QPalette::Base,QBrush(QColor("#dfdfdf")));
    ui->tableWidget->setPalette(widgetpat);
    ui->tableWidget->horizontalHeader()->setPalette(widgetpat);
    startTimer();
    setList();
}

Writecardlist::~Writecardlist()
{
    delete ui;
}
void Writecardlist::writecardlist_GetData()
{
    OperateFile::tracelog("TCP receive:"+socket->ToString());
    QStringList list=socket->ToString().split("_");
    if(list.size()<3)
    {
        return;
    }
    if(list.at(2)=="GetState")
    {
        if(list.at(3)!="0")
        {
           // 获取卡类型失败
            messagebox->closeDialog();//关闭最初对话框
            disconnectSlots();
            QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"您已将原电卡拔出，无法写卡，请到柜台处理");
            return;
        }
        else
        {
            socket->CardType=QString(list.at(4)).toInt();
            socket->sendMsg(QString("Network&9019&1&EC_ReadCard_%1").arg(QString::number(socket->CardType)));
        }
        return;

    }
    if(list.at(2)=="ReadCard")
    {

        //send费控写卡接口
        if(socket->CardType==0)
        {
            QString string;
            OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD",string);
            socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg("0").arg("").arg("")));
            OperateFile::tracelog(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg("0").arg("").arg(""));
        }
        else if(socket->CardType==1)
        {
            QString string;
            OperateFile::readiniFile("INTERFACE.DATA","interface/LKWRITECARD",string);
            socket->HttpSend(QUrl(QString(string).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg("").arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg(socket->Card_Basemsg.FileDate1)));
            OperateFile::tracelog(QString(string).arg(socket->LK_msg.yhdabh).arg(socket->LK_msg.jlbbh).arg(socket->LoginMessage.rybh).arg("").arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg(socket->Card_Basemsg.FileDate1));
        }
        else if(socket->CardType==3)
        {
            QString string;
            socket->Card_Basemsg.szCardNum=list.at(5);
            socket->Card_Basemsg.szRandFromCard=list.at(6);
            OperateFile::readiniFile("INTERFACE.DATA","interface/FKWRITECARD",string);
            socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh)));
            OperateFile::tracelog(QString(string).arg(socket->FK_msg.yhdabh).arg(socket->FK_msg.dbbh).arg(socket->LoginMessage.rybh).arg("1").arg(ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()).arg("1").arg(socket->Card_Basemsg.szCardNum).arg(socket->Card_Basemsg.szRandFromCard).arg(socket->message.gsbh));
        }
    }
     if(list.at(2)=="WriteCard")
    {

        OperateFile::tracelog("writeCard return:"+list.at(3));
        if(list.at(4)!="0")
        {
               messagebox->closeDialog();
               OperateFile::tracelog("购电补卡写卡失败："+socket->ToString());
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"写卡失败，请到柜台处理");
               return ;
        }
        else
        {
            //补卡完成
            messagebox->closeDialog();
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡完成");
            //ui->tableWidget->clearContents();
            //ui->pushButton_2->setEnabled(false);

            /*disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
            disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_carddatasearch(QNetworkReply*)));
            connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
            QString string;
            OperateFile::readiniFile("INTERFACE.DATA","interface/GETFKBKLB",string);
            socket->HttpSend(QUrl(QString(string).arg(socket->Card_msg.yhdabh).arg(socket->fkbkzt_msg.yffid).arg(socket->fkbkzt_msg.djdm).arg(socket->fkbkzt_msg.dbxb).arg(socket->fkbkzt_msg.zhbl).arg("")));
            OperateFile::tracelog("Send jsp:\t"+QString(string).arg(socket->FK_msg.yhdabh).arg(socket->fkbkzt_msg.yffid).arg(socket->fkbkzt_msg.djdm).arg(socket->fkbkzt_msg.dbxb).arg(socket->fkbkzt_msg.zhbl).arg(socket->fkbkzt_msg.djmc).toLocal8Bit());*/

        }
    }
     else if(list.at(2)=="Purchase")
     {
         OperateFile::tracelog("Purchase return:"+list.at(3));
         if(list.at(3)!="0")
        {

              messagebox->closeDialog();
              OperateFile::tracelog("购电补卡充值失败："+socket->ToString());
              QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
              messagebox->displayWithoutButton(this,"写卡失败");
              return ;
         }
        else
        {
            //补卡完成
            messagebox->closeDialog();
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡完成");
            return ;
        }

     }
}
void Writecardlist::changeCheckstate(int row, int column, int pre_row, int pre_column)
{
   QList<QRadioButton*> rad = ui->tableWidget->findChildren<QRadioButton *>();
   rad.at(row)->setChecked(true);
   startTimer();

}
void Writecardlist::MouseTrackItem(int row)
{
   //ui->tableWidget->setStyleSheet("QTableWidget::item{selection-background-color:lightblue;}"); //选中项的颜色
   //qDebug()<<"2222:"<<row;
    ui->tableWidget->setCurrentCell(row, QItemSelectionModel::Select); //设置该行为选中项
    /*for(int j=1;j<8;j++)
    {
        ui->tableWidget->item(row,j)->setTextColor("#ffaa00");
    }*/
    startTimer();
}
void Writecardlist::replyFinished(QNetworkReply *reply)
{

    ui->pushButton_2->setEnabled(true);
    if(reply->error() == QNetworkReply::NoError)
     {
        char m_receivedata[2048];
        memset(m_receivedata,0,2048);
        reply->read(m_receivedata,2048);
        OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
        m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
        QStringList temp_list=QString(m_HttpData.at(0)).split("@");
        QStringList base_arg=QString(temp_list.at(0)).split("!*!");
        rows=m_HttpData.size();
        QString m_string=m_HttpData.at(0);
        setRows();
        if(base_arg.at(0)=="ok")
        {
            if(m_string.size()<4)
            {

               ui->pushButton_2->setEnabled(false);
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"未查询到历史充值记录");
               disconnectSlots();
               //socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
               return;
            }
            QFont font;
            font.setFamily("Mircosoft YaiHei UI");
            font.setPointSize(12);
            //columnHeaderItem0->setFont(font);
            m_string=m_string.remove(0,QString(temp_list.at(0)).size()+1);
            for(int i=0;i<rows;i++)
            {

                QWidget *widget;
                QHBoxLayout *hLayout;
                QRadioButton *button=new QRadioButton(this);
                hLayout = new QHBoxLayout();
                widget = new QWidget(ui->tableWidget);
                hLayout->addWidget(button);
                hLayout->setMargin(0);                          // 必须添加, 否则CheckBox不能正常显示
                hLayout->setAlignment(button, Qt::AlignCenter);
                widget->setLayout(hLayout);
                btnGroup->addButton(button,i);
                ui->tableWidget->setCellWidget(i,0,widget);
                    QString str=m_HttpData.at(i);
                    if(i==0)
                        str=m_string;
                    QStringList list=str.split("@");
                    if(socket->CardType==0||socket->CardType==3)
                    {

                        ui->tableWidget->setItem(i,1,new QTableWidgetItem(list.at(0)));
                        ui->tableWidget->setItem(i,2,new QTableWidgetItem(list.at(1)));
                        ui->tableWidget->setItem(i,3,new QTableWidgetItem(list.at(3)));
                        ui->tableWidget->setItem(i,4,new QTableWidgetItem(list.at(5)));
                        ui->tableWidget->setItem(i,5,new QTableWidgetItem(list.at(7)));
                        ui->tableWidget->setItem(i,6,new QTableWidgetItem(list.at(8)));
                        ui->tableWidget->setItem(i,7,new QTableWidgetItem(list.at(9)));
                    }
                    else
                    {
                        ui->tableWidget->setItem(i,1,new QTableWidgetItem(list.at(0)));
                        ui->tableWidget->setItem(i,2,new QTableWidgetItem(list.at(1)));
                        ui->tableWidget->setItem(i,3,new QTableWidgetItem(list.at(2)));
                        ui->tableWidget->setItem(i,4,new QTableWidgetItem(list.at(3)));
                        ui->tableWidget->setItem(i,5,new QTableWidgetItem(list.at(5)));
                        ui->tableWidget->setItem(i,6,new QTableWidgetItem(list.at(11)));
                        ui->tableWidget->setItem(i,7,new QTableWidgetItem(list.at(12)));
                    }
                for(int j=1;j<8;j++)
                {
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                    //ui->tableWidget->item(i,j)->setFont(font);
                }
            }
            ui->tableWidget->show();
            ui->tableWidget->setCurrentCell(0,1);
        }
        else
        {

            ui->pushButton_2->setEnabled(false);
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"未查询到历史充值记录");
            disconnectSlots();
            socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
            OperateFile::ui_homepage->init();

        }

    }

    else
    {

        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
        OperateFile::ui_homepage->init();
    }
    reply->deleteLater();
}
void Writecardlist::replyFinished_writecard(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
          char m_receivedata[2048];
          memset(m_receivedata,0,2048);
          reply->read(m_receivedata,2048);
          /*对返回串做处理*/
           /*写日志*/
        OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LKWRITECARD)
            {
                LK_writewithcard.gdl=m_HttpData.at(1);
                LK_writewithcard.xkdl=m_HttpData.at(2);
                LK_writewithcard.gdcs=m_HttpData.at(3);
                LK_writewithcard.ccs1=m_HttpData.at(4);
                  //send 量控写卡数据查询
                 disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                 disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                 connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_carddatasearch(QNetworkReply*)));
                 QString string;
                 OperateFile::readiniFile("INTERFACE.DATA","interface/LKDATASEARCH",string);
                 socket->HttpSend(QUrl(QString(string).arg(socket->writecard_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
                 OperateFile::tracelog(QString(string).arg(socket->writecard_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));

            }
            else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::FKWRITECARD)
            {
                FK_writewithcard.jfje=m_HttpData.at(1);
                FK_writewithcard.czje=m_HttpData.at(2);
                FK_writewithcard.czcs=m_HttpData.at(3);
                FK_writewithcard.ccs1=m_HttpData.at(4);
                FK_writewithcard.ccs2=m_HttpData.at(5);
                FK_writewithcard.ccs3=m_HttpData.at(6);
                FK_writewithcard.szRandFromHsm=m_HttpData.at(7);
                FK_writewithcard.szMac1=m_HttpData.at(8);
                FK_writewithcard.szMac2=m_HttpData.at(9);
                FK_writewithcard.szMac3=m_HttpData.at(10);
                FK_writewithcard.szMac4=m_HttpData.at(11);
                FK_writewithcard.szDes1=m_HttpData.at(12);
                FK_writewithcard.szDes2=m_HttpData.at(13);
                FK_writewithcard.szKeyData=m_HttpData.at(14);
                 // 费控写卡数据查询
                disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
                disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
                connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_carddatasearch(QNetworkReply*)));
                QString string;
                OperateFile::readiniFile("INTERFACE.DATA","interface/FKDATASEARCH",string);
                socket->HttpSend(QUrl(QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2))));
                OperateFile::tracelog(QString(string).arg(socket->FK_msg.yhdabh).arg(QString(socket->LoginMessage.gsbh).left(2)));
                 //socket->HttpSend(QUrl("http://122.113.5.232:8040/zzjfnew/get_fkkx_je.jsp?xmlData=<DATA><YHDABH>020100001487</YHDABH><QYBH>02</QYBH></DATA>"));

            }
         }
         else if(base_arg.at(0)=="error")
          {
               messagebox->closeDialog();
               OperateFile::tracelog(QString::fromLocal8Bit(m_receivedata));
               QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
               messagebox->displayWithoutButton(this,"获取写卡数据失败，无法补卡");
          }
    }
    else
    {

        OperateFile::tracelog("server refuse");
        messagebox->closeDialog();
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
    }
    reply->deleteLater();
}
void Writecardlist::replyFinished_carddatasearch(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
         char m_receivedata[2048];
         memset(m_receivedata,0,2048);
         reply->read(m_receivedata,2048);
         /*对返回串做处理*/
          /*写日志*/
        OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
        m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
        QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
        if(base_arg.at(0)=="ok")
        {
            if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
            {
                OperateFile::tracelog("error argument");
                return;
            }
           if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::FKDATASEARCH)
           {
               FK_datacheck.bcgdje=m_HttpData.at(1);
               FK_datacheck.bcgdye=m_HttpData.at(2);
               FK_datacheck.fpqybz=m_HttpData.at(3);
               if(socket->CardType==0)
                socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4").arg(socket->CardType).arg(FK_writewithcard.ccs1).arg(FK_writewithcard.ccs2).arg(FK_writewithcard.ccs3));
               else
                socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4_%5_%6_%7_%8_%9_%10_%11_%12").arg(socket->CardType).arg(FK_writewithcard.ccs1).arg(FK_writewithcard.ccs2)\
                                   .arg(FK_writewithcard.ccs3).arg(FK_writewithcard.szRandFromHsm).arg(FK_writewithcard.szMac1).arg(FK_writewithcard.szMac2).arg(FK_writewithcard.szMac3)\
                                   .arg(FK_writewithcard.szMac4).arg(FK_writewithcard.szDes1).arg(FK_writewithcard.szDes2).arg(FK_writewithcard.szKeyData));

           }
           else if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::LKDATASEARCH)
           {
               //量控数据查询
               LK_datacheck.bcgdje=m_HttpData.at(1);
               LK_datacheck.bcgdl=m_HttpData.at(2);
               LK_datacheck.bcgdye=m_HttpData.at(3);
               LK_datacheck.fpqybz=m_HttpData.at(4);
               socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2").arg(socket->CardType).arg(LK_writewithcard.ccs1));

           }
        }

        else if(base_arg.at(0)=="error")
        {
            OperateFile::tracelog(QString::fromLocal8Bit(m_receivedata));
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"写卡数据查询失败，无法补卡");
        }
    }
    else{
        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
        OperateFile::ui_homepage->init();
    }

    reply->deleteLater();
}
void Writecardlist::setList()
{
  ui->tableWidget->setColumnCount(8);
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  QStringList header;
  if(socket->CardType==0||socket->CardType==3)
    header<<"选择"<<"任务编号"<<"用户档案编号"<<"凭单编号"<<"缴费金额"<<"充值次数"<<"充值金额"<<"记录时间";
  else
    header<<"选择"<<"记录序号"<<"用户档案编号"<<"购电金额"<<"购电量"<<"计量表编号"<<"充值次数"<<"购电时间";
  ui->tableWidget->setHorizontalHeaderLabels(header);
  ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列表头
  ui->tableWidget->verticalHeader()->setDefaultSectionSize(33); //设置行距
  ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray; background:#dfdfdf;}");
  //ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
  ui->tableWidget->horizontalHeader()->setHighlightSections(false);
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并平分
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->setFrameShape(QFrame::NoFrame);

  QFont font=ui->tableWidget->horizontalHeader()->font();
  for(int i=0;i<8;i++)
  {
      QTableWidgetItem *columnHeaderItem0 = ui->tableWidget->horizontalHeaderItem(i); //获得水平方向表头的Item对象
      columnHeaderItem0->setTextColor(QColor("#474040")); //设置文字颜色;
      font.setFamily("Mircosoft YaiHei UI");
      font.setBold(true);
      font.setPointSize(13);
      columnHeaderItem0->setFont(font);
  }

  ui->tableWidget->show();
}

/*void Writecardlist::on_pushButton_clicked()
{
    write_button=true;
    messagebox->displayWithoutBlock(this,"正在写卡，请稍后……");

            //QString sendstr=QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4_%5_%6_%7_%8").arg(unwrite_msg.guiyue).arg("1").arg(unwrite_msg.byteof8).arg(unwrite_cardmsg.cardNum).arg(unwrite_msg.fileParam).arg(unwrite_msg.fileMoney).arg(unwrite_msg.filePrice1).arg(unwrite_msg.filePrice2);
            socket->sendMsg(QString("Network&9019&1&EC_WriteCard_%1_%2_%3_%4_%5_%6_%7_%8").arg(unwrite_msg.guiyue).arg(unwrite_msg.writetype).arg(unwrite_msg.byteof8).arg(unwrite_cardmsg.cardNum).arg(unwrite_msg.fileParam).arg(unwrite_msg.fileMoney).arg(unwrite_msg.filePrice1).arg(unwrite_msg.filePrice2));

}*/
void Writecardlist::on_pushButton_2_clicked()
{
    write_button=false;
    messagebox->displayWithoutBlock(this,"正在写卡，请稍后……");
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_carddatasearch(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)),Qt::UniqueConnection);
    socket->sendMsg("Network&9019&1&EC_GetState");
}
/*void Writecardlist::getitem()
{
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
    int count=items.count();
    for(int i=0;i<count;i++)
        {
           QTableWidgetItem*item=items.at(i);
           if(i==0)
            selecttext=item->text();//获取内容
           else
            selecttext=selecttext+"@"+item->text();
        }
}*/
void Writecardlist::waitTimeout()
{
    emit updateDispaly();
    ui->label_10->show();
    ui->lcdNumber->show();
}
void Writecardlist::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_10->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
void Writecardlist::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->label_10->hide();
    ui->lcdNumber->hide();
    emit start(MSEC);
}
void Writecardlist::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
       disconnectSlots();
       socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
       OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
void Writecardlist::disconnectSlots()
{
    emit stop();
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_writecard(QNetworkReply*)));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished_carddatasearch(QNetworkReply*)));
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(writecardlist_GetData()));
}
void Writecardlist::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
    OperateFile::ui_homepage->init();
}
void Writecardlist::setRows()
{
   ui->tableWidget->setRowCount(rows);    //设置行数为10
  // ui->tableWidget->setCurrentCell(0,1);
}

void Writecardlist::on_pushButton_clicked()
{
     disconnectSlots();
     socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
     OperateFile::ui_homepage->init();

}

void Writecardlist::on_pushButton_home_clicked()
{
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE,NEIYE);
    OperateFile::ui_homepage->init();
}

void Writecardlist::on_pushButton_purchase_clicked()
{
    ui->pushButton_search->setEnabled(false);
    socket->IsPurchase=false;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void Writecardlist::on_pushButton_search_clicked()
{
    disconnectSlots();

    if(OperateFile::ui_lookup==0)
    {
        OperateFile::ui_lookup=new Lookup();
    }
    socket->effect->begin(this, OperateFile::ui_lookup,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_lookup->init();
}

void Writecardlist::on_pushButton_public_clicked()
{

}
