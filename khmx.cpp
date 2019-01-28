#include "khmx.h"
#include "ui_khmx.h"
#include"clientsocket.h"
#include"homepage.h"
#include"first.h"
#include"manager.h"
#include"lookup.h"
#include"readcard.h"
#include<QCalendarWidget>
#include<QScrollBar>
extern Manager *ui_manager;
extern First ui_first;
extern ClientSocket *socket;
extern HomePage *homepage;
KHmx::KHmx(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KHmx)
{
    ui->setupUi(this);
    ui->frame_2->setObjectName("frame2");
    ui->frame_6->setObjectName("frame6");
    ui->frame->setObjectName("frame1");
    ui->frame->setStyleSheet("QFrame#frame1{background-color:#dfdfdf;border:1px;border-radius:2px;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{background-color:#dfdfdf;border:1px;border-radius:2px;}");
    ui->frame_6->setStyleSheet("QFrame#frame6{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    /*ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 2px;}"
      "border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:{background:gray;}"
      "QScrollBar::add-line:vertical {border: 2px solid grey;height: 18px;image:url(:/image/picture/qietu/xiala.png);subcontrol-position: bottom;subcontrol-origin: margin;background-color:#ebebeb;"
      "QScrollBar::sub-line:vertical {border: 2px solid grey;height: 18px;image:url(:/image/picture/qietu/shangla.png);subcontrol-position: top;subcontrol-origin: margin;background-color:#ebebeb;}"
       "QScrollBar::up-arrow:vertical{width: 3px;height: 3px;}"
      "QScrollBar::down-arrow:vertical {width: 3px;height: 3px;}" );*/
    ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/anniu2.png);}"
                                    "QPushButton:disabled{border-image: url(:/image/picture/qietu/disable_button.png);}");
    ui->pushButton_3->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/anniu2.png);}"
                                    "QPushButton:disabled{border-image: url(:/image/picture/qietu/disable_button.png);}");
    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_03.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_03.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_05.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_05.png);}");
    ui->pushButton_search->setEnabled(false);
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用+_09.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_09.png);}"
                                    "QPushButton:disabled{border-image: url(:/image/picture/qietu/切图用+_09.png);}");

    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/切图用_07.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/切图用+_07.png);}");

    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
     //调色板
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);

    ui->tableWidget->installEventFilter(this);

    ui->pushButton->setEnabled(false);
    ui->dateEdit_2->setAutoFillBackground(true);
    ui->dateEdit_3->setAutoFillBackground(true);
    ui->dateEdit_2->calendarWidget()->setFixedSize(300,300);
    ui->dateEdit_3->calendarWidget()->setFixedSize(300,300);
    /********设置日期*******/

    ui->dateEdit_3->setDateTime(QDateTime::currentDateTime());
    QString date=ui->dateEdit_3->text();
    QStringList datelist=date.split("-");
    int year=QString(datelist.at(0)).toInt()-1;
    int month=QString(datelist.at(1)).toInt();
    int day=QString(datelist.at(2)).toInt();
    ui->dateEdit_2->setDate(QDate(year,month,day));
    ui->dateEdit_2->setMinimumDate(QDate(year,month,day));
    ui->dateEdit_2->setMaximumDate(QDate::currentDate());
    ui->dateEdit_3->setMinimumDate(QDate(year,month,day));
    ui->dateEdit_3->setMaximumDate(QDate::currentDate());
    /**********************/
    setCalendarStyle();
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    m_currenttime=SEC;
    TotalPageSum=0;
    TotalResultSum=0;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label_6->hide();
    setList();
    startTimer();
    QString string;
    OperateFile::readiniFile("INTERFACE.DATA","interface/GETKHMX",string);

    socket->HttpSend(QUrl(QString(string).arg(socket->message.yhdabh).arg(ui->dateEdit_2->text()).arg(ui->dateEdit_3->text())));
    OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->message.yhdabh).arg(ui->dateEdit_2->text()).arg(ui->dateEdit_3->text()));


}

KHmx::~KHmx()
{
    delete ui;
}

void KHmx::on_pushButton_clicked()
{
    startTimer();
    QString string;
    OperateFile::readiniFile("INTERFACE.DATA","interface/GETKHMX",string);

    socket->HttpSend(QUrl(QString(string).arg(socket->message.yhdabh).arg(ui->dateEdit_2->text()).arg(ui->dateEdit_3->text())));
    OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(socket->message.yhdabh).arg(ui->dateEdit_2->text()).arg(ui->dateEdit_3->text()));

}

void KHmx::setCalendarStyle()
{

        QString strStyle;
        if ( strStyle.isEmpty() )
         {
            strStyle.append("QToolButton#qt_calendar_monthbutton,#qt_calendar_yearbutton{ ");//月、年按钮的字体
            strStyle.append("    color:#FFFFFF; ");
            strStyle.append(" font: 13pt; "  );
            strStyle.append("} ");
            strStyle.append("QToolButton#qt_calendar_prevmonth,#qt_calendar_nextmonth{ ");//需要设置前后月按钮图标的大小
            strStyle.append("    icon-size:38px 30px; ");
            strStyle.append("    border:0px solid; ");
            strStyle.append("} ");
            strStyle.append("QCalendarWidget QWidget#qt_calendar_navigationbar{ ");
            strStyle.append("background-color: #1C8DC7; ");
            strStyle.append("min-height:26px; ");
            strStyle.append("max-height:26px; ");
            strStyle.append("} ");
            strStyle.append("QCalendarWidget QTableView  ");
            strStyle.append("{ ");
            strStyle.append("    alternate-background-color:#DEDEDE; ");
            strStyle.append("    background-color: white; ");
            strStyle.append("} ");
            strStyle.append("QCalendarWidget QMenu { ");
            strStyle.append("     background-color: white;  ");
            strStyle.append("     border: 1px solid white; ");
            strStyle.append(" } ");
            strStyle.append("QCalendarWidget  QMenu::item { ");
            strStyle.append("     background-color: transparent; ");
            strStyle.append("     padding:4px 10px; ");
            strStyle.append("     margin:0px 1px; ");
            strStyle.append("     border-bottom:1px solid #DBDBDB; ");
            strStyle.append("     color: #444444; ");
            strStyle.append("     font: 12pt;");
            strStyle.append(" } ");
            strStyle.append(" QMenu::item:selected { ");
            strStyle.append("     background-color:  #DEDEDE; "); //这一句是设置菜单项鼠标经过选中的样式
            strStyle.append(" } ");
            strStyle.append("QCalendarWidget QAbstractItemView:enabled  ");
            strStyle.append("{ ");
            strStyle.append("  font: 13pt; ");
            strStyle.append("  color: #444444;   ");
            strStyle.append("  selection-background-color: #BCDDEE;  ");
            strStyle.append("  selection-color: #444444;  ");
            strStyle.append("} ");

            strStyle.append("QCalendarWidget  QSpinBox { ");
            strStyle.append("     background-color: transparent; ");
            strStyle.append("    padding-right: 5px; ");
            strStyle.append("    padding-left: 5px; ");
            strStyle.append("     width: 50px;");
            strStyle.append("     font:10pt;");
            strStyle.append("     height: 15px; ");
            strStyle.append(" } ");

            strStyle.append("QCalendarWidget  QSpinBox::up-button { ");
            strStyle.append("     subcontrol-origin:border; ");
            strStyle.append("     subcontrol-position:right; ");
            strStyle.append("     width: 15px;");
            strStyle.append("     height: 18px; ");
            strStyle.append("image: url(:/image/picture/qietu/right.png);");
            strStyle.append(" } ");
            strStyle.append("QCalendarWidget  QSpinBox::down-button { ");
            strStyle.append("     subcontrol-origin:border; ");
            strStyle.append("     subcontrol-position:left; ");
            strStyle.append("image: url(:/image/picture/qietu/left.png);");
            strStyle.append("     width: 15px;");
            strStyle.append("     height: 18px; ");
            strStyle.append(" } ");

            strStyle.append("QCalendarWidget QAbstractItemView:disabled { color:#C0C0C0; ");
            strStyle.append(" } ");
         }
        ui->dateEdit_2->calendarWidget()->setStyleSheet(strStyle);
        ui->dateEdit_3->calendarWidget()->setStyleSheet(strStyle);
}
void KHmx::replyFinished(QNetworkReply * reply)
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    CurrentPageIndex=1;
    if(reply->error() == QNetworkReply::NoError)
     {
         char m_receivedata[4096*100];
         memset(m_receivedata,0,4096*100);
         reply->read(m_receivedata,4096*100);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));

        m_HttpData=QString::fromLocal8Bit(m_receivedata).split("!#!");
        QStringList temp_list=QString(m_HttpData.at(0)).split("@");
        QStringList base_arg=QString(temp_list.at(0)).split("!*!");

        if(base_arg.at(0)=="ok")
        {
            TotalResultSum=QString(base_arg.at(2)).toInt();
            if(TotalResultSum==0)
            {
                ui->pushButton_3->setEnabled(false);
                ui->tableWidget->clearContents();
                ui->label_totalpagesum->setText(QString::number(0));
                ui->label_totalresultsum->setText(QString::number(0));
                ui->label_currentpageindex->setText(QString::number(0));
                return;
            }
            TotalPageSum=(TotalResultSum%9>0)?(TotalResultSum/9+1):(TotalResultSum/9);
            QString m_string=m_HttpData.at(0);
            ui->label_totalpagesum->setText(QString::number(TotalPageSum));
            ui->label_totalresultsum->setText(QString::number(TotalResultSum));

            ui->label_currentpageindex->setText(QString::number(CurrentPageIndex));
            setRows();
            m_string=m_string.remove(0,QString(temp_list.at(0)).size()+1);
            for(int i=0;i<(TotalResultSum>9?9:TotalResultSum);i++)
            {
                QString str=m_HttpData.at(i);
                if(i==0)
                    str=m_string;
                QStringList list=str.split("@");
                for(int j=0;j<6;j++)
                {
                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(list.at(j)));
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                }
            }
            if(TotalResultSum<=9)
            {
                ui->pushButton_3->setEnabled(false);
            }
        }

      }
      reply->deleteLater();
}
void KHmx::flushPage(int flag)
{
    ui->tableWidget->clearContents();
    QString m_string=m_HttpData.at(0);
    QStringList temp_list=QString(m_HttpData.at(0)).split("@");
    m_string=m_string.remove(0,QString(temp_list.at(0)).size()+1);
    int k=0;
    for(int i=9*(CurrentPageIndex+flag);i<9*(CurrentPageIndex+flag)+((TotalResultSum-9*(CurrentPageIndex+flag))>9?9:(TotalResultSum-9*(CurrentPageIndex+flag)));i++)
    {
        QString str=m_HttpData.at(i);
        if(i==0)
            str=m_string;
        QStringList list=str.split("@");

         for(int j=0;j<6;j++)
         {
                ui->tableWidget->setItem(k,j,new QTableWidgetItem(list.at(j)));
                ui->tableWidget->item(k,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

         }
         k++;
    }
    if(flag==0)
    {
        ui->pushButton_2->setEnabled(true);
        CurrentPageIndex=CurrentPageIndex+1;
        ui->label_currentpageindex->setText(QString::number(CurrentPageIndex));
        if(CurrentPageIndex>=TotalPageSum)
            ui->pushButton_3->setEnabled(false);

    }
    else
    {
        ui->pushButton_3->setEnabled(true);
        CurrentPageIndex=CurrentPageIndex-1;
        ui->label_currentpageindex->setText(QString::number(CurrentPageIndex));
        if(CurrentPageIndex==1)
            ui->pushButton_2->setEnabled(false);

    }

}
void KHmx::setList()
{
   /* QPalette widgetpat=ui->tableWidget->palette();
    widgetpat.setBrush(QPalette::Base,QBrush(QColor("#dfdfdf")));
    ui->tableWidget->setPalette(widgetpat);
    ui->tableWidget->horizontalHeader()->setPalette(widgetpat);*/
  ui->tableWidget->setColumnCount(6);   //设置列数为6
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  QStringList header;
  header<<"序号"<<"日期"<<"摘要"<<"缴费金额"<<"应缴电费"<<"账户余额";
  ui->tableWidget->setHorizontalHeaderLabels(header);
  ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列表头
  ui->tableWidget->verticalHeader()->setDefaultSectionSize(33); //设置行距
  ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-left: 0px solid gray;border-right:0px;border-top:0px;border-bottom:1px solid lightgray; background:#dfdfdf;}");
  //ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightblue;}");
  ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并平分

  ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->setFrameShape(QFrame::NoFrame);
  ui->tableWidget->setCurrentItem(ui->tableWidget->item(1,0));
  QFont font=ui->tableWidget->horizontalHeader()->font();
  for(int i=0;i<6;i++)
  {
      QTableWidgetItem *columnHeaderItem0 = ui->tableWidget->horizontalHeaderItem(i); //获得水平方向表头的Item对象
      //columnHeaderItem0->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色
      columnHeaderItem0->setTextColor(QColor("#474040")); //设置文字颜色
      font.setFamily("Mircosoft YaiHei UI");
      font.setBold(true);
      font.setPointSize(13);
      columnHeaderItem0->setFont(font);
  }
      ui->tableWidget->show();
}
void KHmx::setRows()
{
   ui->tableWidget->setRowCount(9);
   ui->tableWidget->setCurrentCell(0,1);
}
void KHmx::on_pushButton_backHomePage_3_clicked()
{
    ui->pushButton_backHomePage_3->setEnabled(false);
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    socket->effect->begin(this,OperateFile::ui_lookup,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_lookup->init();
}
void KHmx::waitTimeout()
{
    emit updateDispaly();
    ui->lcdNumber->show();
    ui->label_6->show();
}
void KHmx::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->lcdNumber->hide();
    ui->label_6->hide();
    emit start(LSEC);
}
void KHmx::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->lcdNumber->hide();
    ui->label_6->hide();
    emit start(LSEC);
}
void KHmx::setLcdnum()
{
    if(ui->lcdNumber->value()==0)
    {
        disconnectSlots();
        socket->effect->begin(this,OperateFile::ui_homepage,LEFTTORIGHT,NONE,CLOSE,NEIYE);
        OperateFile::ui_homepage->init();
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
bool KHmx::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->tableWidget)
    {
        if (event->type()==QEvent::MouseMove||QEvent::MouseButtonPress)
        {
             startTimer();
        }
    }
    return QWidget::eventFilter(watched,event);
}

void KHmx::disconnectSlots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}
void KHmx::on_pushButton_3_clicked()
{
    flushPage(0);
}

void KHmx::on_pushButton_2_clicked()
{
    flushPage(-2);
}
void KHmx::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void KHmx::on_pushButton_purchase_clicked()
{
    ui->pushButton_purchase->setEnabled(false);
    socket->IsPurchase=true;
    disconnectSlots();
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    socket->effect->begin(this,OperateFile::ui_readcard,LEFTTORIGHT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void KHmx::on_pushButton_search_clicked()
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

void KHmx::on_pushButton_public_clicked()
{

}
