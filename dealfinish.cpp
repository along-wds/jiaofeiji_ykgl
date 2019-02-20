#include "dealfinish.h"
#include "ui_dealfinish.h"
#include"readcard.h"
#include"inputpassword.h"
#include"inputamount.h"
#include"unionpay.h"
#include"dealfinish.h"
#include"cashpay.h"
dealfinish::dealfinish(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dealfinish)
{
    ui->setupUi(this);
    ui->frame_5->setObjectName("frame5");
    ui->frame_2->setObjectName("frame2");
    ui->frame_8->setObjectName("frame8");
    ui->frame->setObjectName("frame1");
    ui->frame_4->setObjectName("frame4");
    ui->frame_4->setStyleSheet("QFrame#frame4{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame->setStyleSheet("QFrame#frame1{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray;}");
    ui->frame_8->setStyleSheet("QFrame#frame8{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->frame_2->setStyleSheet("QFrame#frame2{background-color:#dfdfdf;border:1px;border-radius:3px;}");
    ui->pushButton_backHomePage_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_03.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_03.png);}");
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_05.png);}"
                                  "QPushButton:disabled{border-image: url(:/image/picture/qietu/切图用+_05.png);}");
    ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_09.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_09.png);}");
    ui->pushButton_4->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_07.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_07.png);}");
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}
void dealfinish::init(int initType)
{
    ui->pushButton->setEnabled(false);
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket,SIGNAL(readyRead()),this,SLOT(dealfinish_GetData()),Qt::UniqueConnection);
    startTimer();
    socket->DataBase.insertData(socket->message.yhdabh,socket->message.yhmc,"",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),socket->YL_msg.kczje,"电力","成功","成功","");
    switch (initType) {
    case 0: //正常写卡
        display();
        break;
    case 1:
        display_with_WriteCard();
        break;
    case 2:
        display();
        ui->label_3->setText("恭喜缴费成功");
        break;
    case 3:
        display_without_writecard();
        break;
    default:
        break;
    }
}
dealfinish::~dealfinish()
{
    delete ui;
}
void dealfinish::display()
{
    /*****************************************/
        //显示内容
    /****************************************/
    if((socket->CardType==1&&socket->LK_datacheck.fpqybz=="1")||(socket->CardType==0&&socket->FK_datacheck.fpqybz=="1"))
    {
        ui->pushButton_5->setEnabled(true);
    }
    else
        ui->pushButton_5->hide();
    int namesize=socket->message.yhmc.size();
    int phonesize=socket->message.phone.size();
    int sfzhsize=socket->message.sfznum.size();
    QString Displayname=socket->message.yhmc;
    QString Displayphone=socket->message.phone;
    QString Displaysfzh;
    if(namesize!=0)
    {
         Displayname=Displayname.replace(0,1,"*");
    }
    if(phonesize!=0)
    {
        Displayphone=socket->message.phone.left(3)+socket->message.phone.right(4);
        for(int i=0;i<phonesize-7;i++)
        {
            Displayphone.insert(3+i,"*");
        }
    }
     if(sfzhsize!=0&&sfzhsize>7)
      {
          Displaysfzh=socket->message.sfznum.left(3)+socket->message.sfznum.right(4);
          for(int i=0;i<sfzhsize-7;i++)
          {
             Displaysfzh.insert(3+i,"*");
          }
      }
     switch (socket->CardType) {
     case 2:
         // 普通户
         if(socket->message.fffs=="01")
         {

             ui->label_29->setText("      本次余额:");
             ui->label_30->setText("     上次余额:");
             ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
             ui->label_wyj->setText(socket->YL_msg.sjwyj);
             ui->label_bcgdye->setText(socket->YL_msg.bcye);//本次余额
             ui->label_10->hide();
             ui->label_30->hide();
             ui->label_bcgdje->hide();
             ui->frame_6->hide();

         }
         //无卡费控户
         else if(socket->message.fffs=="04")
         {
             ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
             ui->label_wyj->setText(socket->YL_msg.sjwyj);
             ui->label_bcgdje->setText(socket->YL_msg.bcgdje);
             ui->label_bcgdye->setText("0");
             ui->frame_6->hide();

         }
         //无卡量控户
         else if(socket->message.fffs=="02")
         {
             ui->label_28->setText("   本次购电量:");
             ui->label_5->setText("kWh");
             ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
             ui->label_wyj->setText("0");
             ui->label_bcgdye->setText("0");
             ui->label_10->hide();
             ui->label_30->hide();
             ui->label_bcgdje->hide();
             ui->frame_6->hide();
         }
         else
         {
             ui->label_bcsjje->setText(socket->YL_msg.bcsjje);//实缴金额
             ui->label_wyj->setText(socket->YL_msg.sjwyj);
             ui->frame_3->hide();
             ui->frame_6->hide();
         }
         break;
     case 0:
     case 3:
         ui->label_3->setText("恭喜写卡成功");
         ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
         ui->label_wyj->setText(socket->YL_msg.sjwyj);
         ui->label_bcgdje->setText(socket->FK_datacheck.bcgdje);
         ui->label_bcgdye->setText(socket->FK_datacheck.bcgdye);
         ui->label_czcs->setText(socket->FK_writewithcard.czcs);
         ui->label_czje->setText(socket->FK_writewithcard.czje);
         break;
     case 1:
         ui->label_3->setText("恭喜写卡成功");
         ui->label_28->setText("   本次购电量:");
         ui->label_31->setText("         购电量:");
         ui->label_32->setText("       购电次数:");
         ui->label_5->setText("kWh");
         ui->label_11->setText("kWh");
         ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
         ui->label_wyj->setText(socket->LK_datacheck.bcgdl);
         ui->label_bcgdje->setText(socket->LK_datacheck.bcgdje);
         ui->label_bcgdye->setText(socket->LK_datacheck.bcgdye);
         ui->label_czje->setText(socket->LK_writewithcard.gdl);
         ui->label_czcs->setText(socket->LK_writewithcard.gdcs);
         break;
     default:
         break;
     }

    ui->label_ClientName->setText(socket->message.yhmc);
    ui->label_ClientNum->setText(socket->message.yhdabh);
    ui->label_ContactTelephone_2->setText(Displayphone);
    ui->label_address->setText(socket->message.Address);
}
void dealfinish::display_with_WriteCard()
{

    int namesize=socket->message.yhmc.size();
    int phonesize=socket->message.phone.size();
    int sfzhsize=socket->message.sfznum.size();
    QString Displayname=socket->message.yhmc;
    QString Displayphone=socket->message.phone;
    QString Displaysfzh;
    if(namesize!=0)
    {
         Displayname=Displayname.replace(0,1,"*");
    }
    if(phonesize!=0)
    {
        Displaysfzh=socket->message.phone.left(3)+socket->message.phone.right(4);
        for(int i=0;i<phonesize-7;i++)
        {
            Displayphone.insert(3+i,"*");
        }
    }
     if(sfzhsize!=0&&sfzhsize>7)
      {
          Displaysfzh=socket->message.sfznum.left(3)+socket->message.sfznum.right(4);
          for(int i=0;i<sfzhsize-7;i++)
          {
             Displaysfzh.insert(3+i,"*");
          }
      }
     ui->label_3->setText("恭喜写卡成功");
     ui->label_25->hide();
     ui->label_ContactTelephone_2->hide();
     ui->frame_10->hide();
     ui->frame_3->hide();
     switch (socket->CardType)
     {
     case 0:
     case 3:
         ui->label_czcs->setText(socket->FK_writewithcard.czcs);
         ui->label_czje->setText(socket->FK_writewithcard.czje);
         break;
     case 1:
         ui->label_5->setText("kWh");
         ui->label_11->setText("kWh");
         ui->label_31->setText("     总购电量:");
         ui->label_32->setText("      购电次数:");
         ui->label_czje->setText(socket->LK_writewithcard.gdl);
         ui->label_czcs->setText(socket->LK_writewithcard.gdcs);
         break;
     default:
         break;
     }
}
void dealfinish::display_without_writecard()
{
    qDebug()<<"display";
    /*****************************************/
        //显示内容
    /****************************************/
    if((socket->CardType==1&&socket->LK_datacheck.fpqybz=="1")||(socket->CardType==0&&socket->FK_datacheck.fpqybz=="1"))
    {
        ui->pushButton_5->setEnabled(true);
    }
    else
        ui->pushButton_5->hide();
    int namesize=socket->message.yhmc.size();
    int phonesize=socket->message.phone.size();
    int sfzhsize=socket->message.sfznum.size();
    QString Displayname=socket->message.yhmc;
    QString Displayphone=socket->message.phone;
    QString Displaysfzh;
    if(namesize!=0)
    {
         Displayname=Displayname.replace(0,1,"*");
    }
    if(phonesize!=0)
    {
        Displaysfzh=socket->message.phone.left(3)+socket->message.phone.right(4);
        for(int i=0;i<phonesize-7;i++)
        {
            Displayphone.insert(3+i,"*");
        }
    }
     if(sfzhsize!=0&&sfzhsize>7)
      {
          Displaysfzh=socket->message.sfznum.left(3)+socket->message.sfznum.right(4);
          for(int i=0;i<sfzhsize-7;i++)
          {
             Displaysfzh.insert(3+i,"*");
          }
      }
     //无卡费控户
     if(socket->message.fffs=="01")
     {

         ui->label_29->setText("      本次余额:");
         ui->label_30->setText("     上次余额:");
         ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
         ui->label_wyj->setText(socket->YL_msg.sjwyj);
         ui->label_bcgdye->setText(socket->YL_msg.bcye);//本次余额
         ui->label_10->hide();
         ui->label_30->hide();
         ui->label_bcgdje->hide();
         ui->frame_6->hide();

     }
     else if(socket->message.fffs=="04")
     {
         ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
         ui->label_wyj->setText(socket->YL_msg.sjwyj);
         ui->label_bcgdje->setText(socket->YL_msg.bcgdje);
         ui->label_bcgdye->setText("0");
         ui->frame_6->hide();

     }
     //无卡量控户
     else  if(socket->message.fffs=="02")
     {
         ui->label_28->setText("   本次购电量:");
         ui->label_5->setText("kWh");
         ui->label_bcsjje->setText(socket->YL_msg.sjje);//实缴金额
         ui->label_wyj->setText("0");
         ui->label_bcgdje->setText(socket->YL_msg.bcgdje);
         ui->label_bcgdye->setText("0");
         ui->label_10->hide();
         ui->label_30->hide();
         ui->label_bcgdje->hide();
         ui->frame_6->hide();
     }
     ui->label_ClientName->setText(socket->message.yhmc);
     ui->label_ClientNum->setText(socket->message.yhdabh);
     ui->label_ContactTelephone_2->setText(Displayphone);
     ui->label_address->setText(socket->message.Address);
}
void dealfinish::dealfinish_GetData()
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
            socket->sendMsg("Network&9008&1&Printer_Print_"+PrintString);
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
void dealfinish::get_InvoicePrinter_Data()
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
            socket->sendMsg("Network&9008&1&Printer_Print_"+PrintString);
        }
        else
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法打印发票");
        }
    }
    if(list.at(1)=="print")
    {
        if(list.at(2)!="ok")
        {
            OperateFile::tracelog("打印出错，错误码："+list.at(3));
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"无法打印发票");
        }
    }
}
void dealfinish::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         reply->read(m_receivedata,4096);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         qDebug()<<m_HttpData.at(1);
         QStringList content_list=QString(m_HttpData.at(1)).split("$");
         QString tempstring;
         QString CurrentTime,currentdevid;
         OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",currentdevid);
         CurrentTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
         PrintString.append("        内蒙古电力自助缴费终端消费凭条\r\n");
         PrintString.append("----------------------------------------------\r\n");
         PrintString.append("用户名称:"+socket->message.yhmc+"\r\n");
         PrintString.append("用户编号:"+socket->message.yhdabh+"\r\n");
         if(content_list.size()>1)//长串
         {
            header.insert(1,"  名称          电量(kWh)    电价(元/kWh) 电费(元)");
            header.insert(2,"  表号       上期指标   本期指标   倍率    电量");
            //PrintString.append("             "+socket->message.yhmc+"\r\n");

            for(int i=1;i<content_list.size()-1;i++)   //第几部分
            {
                PrintString.append(header.value(i));  //添加表头
                PrintString.append("\r\n");
                QStringList part_list=QString(content_list.at(i)).split("**");
                QStringList part_arg=QString(part_list.at(0)).split("!#!");
                QStringList part_content=QString(part_list.at(1)).split("!#!");
                int li_totalrow=QString(part_arg.at(0)).toInt();
                int li_totalcolumn=QString(part_arg.at(1)).toInt();
                for(int j=0;j<li_totalrow;j++) //第几行
                {
                    for(int k=0;k<li_totalcolumn;k++)//第几列
                    {
                       //qDebug()<<"j:"<<j<<"k;"<<k<<"index:"<<k*j+k<<"111:"<<QString(part_content.at(li_totalcolumn*j+k));
                       QByteArray ba=QString(part_content.at(li_totalcolumn*j+k)).toUtf8();
                       char *p=ba.data();
                       if(k==0)
                       {
                          PrintString.append(tempstring.sprintf("%-16s",p));
                       }
                       else
                       {
                            PrintString.append(tempstring.sprintf("%-10s",p));
                       }

                       //qDebug()<<"tempstring:"<<tempstring;
                       tempstring.clear();
                    }
                    PrintString.append("\r\n");

                }

            }
            QStringList jexx_list=QString(content_list.at(content_list.size()-1)).split("!#!");
            PrintString.append("起始时间: "+jexx_list.at(0)+"  到"+jexx_list.at(1)+"\r\n");

         }
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETPRINTDATA)
            {
                QString jffs=socket->PurchaseType==1?"自助终端_银联缴费":"自助终端_现金缴费";
                PrintString.append("实缴金额: "+socket->YL_msg.sjje+" 元"+"\r\n");
                PrintString.append("本次余额: "+socket->YL_msg.bcye+" 元"+"\r\n");
                PrintString.append("  违约金:"+socket->YL_msg.sjwyj+" 元"+"\r\n");
                PrintString.append("缴费方式:"+jffs+"\r\n");
                PrintString.append("  终端号:"+currentdevid+"\r\n");
                PrintString.append("缴费时间:"+CurrentTime+"\r\n");
                PrintString.append("缴费结果:成功\r\n");
                PrintString.append("----------------------------------------------\r\n");
                PrintString.append(socket->message.fgsmc+"\r\n");
                PrintString.append(socket->message.gsmc+"\r\n");
                PrintString.append("\r\n\r\n\r\n\r\n\r\n");
                //OperateFile::tracelog(PrintString);

                if(ui->pushButton_6->isEnabled()==false)
                    socket->sendMsg("Network&9008&1&Printer_GetState");
                else
                    socket->sendMsg("Network&9009&1&InvoicePrinter_GetState");
            }

            else if(base_arg.at(0)=="error")
            {

                OperateFile::tracelog("打印失败:"+QString(m_HttpData.at(1)));
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"无法打印凭条");
            }
          }
    }
    else{
           OperateFile::tracelog(QString(NETWORKERROR)+"in inputmount");
        }
    reply->deleteLater();

}
void dealfinish::InvoicePrinter_replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096];
         memset(m_receivedata,0,4096);
         reply->read(m_receivedata,4096);
         qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         QStringList base_arg=QString(m_HttpData.at(0)).split("!*!");
         qDebug()<<m_HttpData.at(1);
         QStringList content_list=QString(m_HttpData.at(1)).split("$");
         QString tempstring;
         QString CurrentTime,currentdevid;
         OperateFile::readiniFile(DEVINFO,"TERMINF/TERM_ID",currentdevid);
         CurrentTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
         PrintString.append("          ");
         PrintString.append("F");
         PrintString.append(currentdevid);
         PrintString.append(CurrentTime);
         PrintString.append("\r\n");
         PrintString.append("          ");
         PrintString.append(currentdevid);
         PrintString.append("\r\n");
         PrintString.append("          ");
         PrintString.append("      ");
         PrintString.append("内蒙古电力(集团)有限责任公司");
         PrintString.append(socket->message.fgsmc);
         PrintString.append("\r\n\r\n");
         PrintString.append("      ");
         PrintString.append(CurrentTime);
         PrintString.append("自助缴费机");
         PrintString.append(currentdevid);
         PrintString.append("\r\n");
         PrintString.append("          ");
         PrintString.append(socket->message.yhmc);
         PrintString.append("\r\n");
         if(content_list.size()>1)//长串
         {
            header.insert(1,"  名称          电量(kWh)    电价(元/kWh) 电费(元)");
            header.insert(2,"  表号       上期指标   本期指标   倍率    电量");
            //PrintString.append("             "+socket->message.yhmc+"\r\n");

            for(int i=1;i<content_list.size()-1;i++)   //第几部分
            {
                PrintString.append(header.value(i));  //添加表头
                PrintString.append("\r\n");
                QStringList part_list=QString(content_list.at(i)).split("**");
                QStringList part_arg=QString(part_list.at(0)).split("!#!");
                QStringList part_content=QString(part_list.at(1)).split("!#!");
                int li_totalrow=QString(part_arg.at(0)).toInt();
                int li_totalcolumn=QString(part_arg.at(1)).toInt();
                for(int j=0;j<li_totalrow;j++) //第几行
                {
                    for(int k=0;k<li_totalcolumn;k++)//第几列
                    {
                       //qDebug()<<"j:"<<j<<"k;"<<k<<"index:"<<k*j+k<<"111:"<<QString(part_content.at(li_totalcolumn*j+k));
                       QByteArray ba=QString(part_content.at(li_totalcolumn*j+k)).toUtf8();
                       char *p=ba.data();
                       if(k==0)
                       {
                          PrintString.append(tempstring.sprintf("%-16s",p));
                       }
                       else
                       {
                            PrintString.append(tempstring.sprintf("%-10s",p));
                       }

                       qDebug()<<"tempstring:"<<tempstring;
                       tempstring.clear();
                    }
                    PrintString.append("\r\n");

                }

            }
            QStringList jexx_list=QString(content_list.at(content_list.size()-1)).split("!#!");
            PrintString.append("起始时间: "+jexx_list.at(0)+"  到"+jexx_list.at(1)+"\r\n");

         }
         if(base_arg.at(0)=="ok")
         {
             if(QString(base_arg.at(3)).toInt()+1!=m_HttpData.size())
             {
                 OperateFile::tracelog("error argument");
                 return;
             }
            if(QString(base_arg.at(1)).toInt()==INTERFACETYPE::GETPRINTDATA)
            {
                OperateFile::tracelog(PrintString);
                socket->sendMsg("Network&9009&1&InvoicePrinter_GetState");
            }

            else if(base_arg.at(0)=="error")
            {

                OperateFile::tracelog("打印失败:"+QString(m_HttpData.at(1)));
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"无法打印凭条");
            }
          }
    }
    else{
           OperateFile::tracelog(QString(NETWORKERROR)+"in inputmount");
        }
    reply->deleteLater();
}
void dealfinish::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label->show();
}
//返回首页
void dealfinish::on_pushButton_2_clicked()
{
    ui->pushButton_2->setEnabled(false);
    disconnectSlots();
    if(socket->PurchaseType==0)
    {
#ifdef DEBUG
       OperateFile::ui_inputamount->close();
#else
        OperateFile::tracelog("1111111111111111111");
        OperateFile::ui_cashpay->close();
        OperateFile::tracelog("222222222222222222");
#endif
    }
    else
    {
        OperateFile::ui_inputpassword->close();
        OperateFile::ui_inputamount->close();
        OperateFile::ui_homepage->ejectCard();
    }
    OperateFile::tracelog("33333333333333333333");
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
    OperateFile::tracelog("444444444444444444444");
}
void dealfinish::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label->hide();
    emit start(MSEC);
}
void dealfinish::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label->hide();
    emit start(MSEC);

}
void dealfinish::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        disconnectSlots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
        OperateFile::ui_homepage->init();
        if(socket->PurchaseType==1)
        {
            OperateFile::ui_inputpassword->close();
            OperateFile::ui_inputamount->close();
        }
        else
            OperateFile::ui_cashpay->close();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}

void dealfinish::on_pushButton_backHomePage_2_clicked()
{
    ui->pushButton_backHomePage_2->setEnabled(false);
    disconnectSlots();
    if(socket->PurchaseType==1)
    {
        OperateFile::ui_inputpassword->close();
        OperateFile::ui_inputamount->close();
        OperateFile::ui_homepage->ejectCard();
    }
    else
        OperateFile::ui_cashpay->close();

    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void dealfinish::on_pushButton_back_2_clicked()
{
    ui->pushButton_back_2->setEnabled(false);
    disconnectSlots();
    if(socket->PurchaseType==1)
    {
        OperateFile::ui_inputpassword->close();
        OperateFile::ui_inputamount->close();
        OperateFile::ui_homepage->ejectCard();
    }
    else
        OperateFile::ui_cashpay->close();

    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
void dealfinish::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(dealfinish_GetData()));

}
void dealfinish::on_pushButton_clicked()
{

}
void dealfinish::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    disconnectSlots();

    if(socket->PurchaseType==1)
    {
        OperateFile::ui_inputpassword->close();
        OperateFile::ui_inputamount->close();
        OperateFile::ui_homepage->ejectCard();
    }
    else
        OperateFile::ui_cashpay->close();
    socket->IsPurchase=false;
    socket->effect->begin(this, OperateFile::ui_readcard,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}
void dealfinish::on_pushButton_4_clicked()
{
    ui->pushButton_4->setEnabled(false);
    disconnectSlots();
    OperateFile::ui_inputpassword->close();
    if(socket->PurchaseType==1)
    {
        OperateFile::ui_inputpassword->close();
        OperateFile::ui_inputamount->close();
        OperateFile::ui_homepage->ejectCard();
    }
    else
        OperateFile::ui_cashpay->close();

    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
/*****************打印凭条*******************/
void dealfinish::on_pushButton_6_clicked()
{
    QString string;
    ui->pushButton_6->setEnabled(false);
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(get_InvoicePrinter_Data()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(InvoicePrinter_replyFinished(QNetworkReply*)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(dealfinish_GetData()),Qt::UniqueConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    OperateFile::readiniFile("INTERFACE.DATA","interface/PRINT",string);
    socket->HttpSend(QUrl(QString(string).arg(socket->LoginMessage.rybh).arg(socket->YL_msg.pdbh).arg(socket->message.yhdabh).arg(socket->LoginMessage.gsbh)));
    OperateFile::tracelog("http send:\t"+QString(string).arg(socket->LoginMessage.rybh).arg(socket->YL_msg.pdbh).arg(socket->message.yhdabh).arg(socket->LoginMessage.gsbh));

}

void dealfinish::on_pushButton_5_clicked()
{
    QString string;
    ui->pushButton_5->setEnabled(false);
    disconnect(socket,SIGNAL(readyRead()),this,SLOT(dealfinish_GetData()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(InvoicePrinter_replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    connect(socket,SIGNAL(readyRead()),this,SLOT(get_InvoicePrinter_Data()),Qt::UniqueConnection);
    OperateFile::readiniFile("INTERFACE.DATA","interface/PRINT",string);
    socket->HttpSend(QUrl(QString(string).arg(socket->LoginMessage.rybh).arg(socket->YL_msg.pdbh).arg(socket->message.yhdabh).arg(socket->LoginMessage.gsbh)));
    OperateFile::tracelog("http send:\t"+QString(string).arg(socket->LoginMessage.rybh).arg(socket->YL_msg.pdbh).arg(socket->message.yhdabh).arg(socket->LoginMessage.gsbh));

}
