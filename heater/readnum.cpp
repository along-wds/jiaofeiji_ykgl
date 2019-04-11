#include "readnum.h"
#include "ui_readnum.h"
#include"manager.h"
#include"operatefile.h"
#include<QMessageBox>
#include"first.h"
#include"QListView"
#include<QListWidget>
extern ClientSocket *socket;
extern Manager *ui_manager;
Readnum::Readnum(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Readnum)
{

    ui->setupUi(this);
    ui->frame_2->setObjectName("frame1");
    ui->frame->setObjectName("frame2");
    ui->frame_2->setStyleSheet("QFrame#frame1{border-image: url(:/image/picture/qietu/20.png);}");
    ui->frame->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/shangdaohang.png);}");
    OperateFile::ui_heaterkhmx=new Heaterkhmx;
    OperateFile::ui_heatermsgconfirm=new Heatermsgconfirm();
    btnGroup=new QButtonGroup(this);
    btnGroup->addButton(ui->radioButton,0);
    btnGroup->addButton(ui->radioButton_2,1);
    ui->lineEdit_number->setPlaceholderText("请输入房屋编号");
    ui->lineEdit->setPlaceholderText("楼号");
    ui->lineEdit_2->setPlaceholderText("门牌号");
    ui->lineEdit_number->setMaxLength(12);
    ui->lineEdit->setMaxLength(4);
    ui->lineEdit_2->setMaxLength(4);
    ui->lineEdit->setFrame(false);
    ui->lineEdit_number->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);  //在窗体上为lineEdit1安装过滤器
    ui->lineEdit_2->installEventFilter(this);//在窗体上为lineEdit2安装过滤
    QStringList xqbh_list=socket->qybh_xqbh_list.values(socket->qybh_qymc_list.key("奎屯市"));
    /*foreach (QString string, xqbh_list)
    {
       ui->comboBox->addItem(socket->xqbh_xqmc_list.value(string));
    }*/
    for(int i=xqbh_list.size()-1;i>0;i--)
    {

      ui->comboBox->addItem(socket->xqbh_xqmc_list.value(QString(xqbh_list.at(i))));
    }
    ui->comboBox->setLineEdit(new QTComboBoxButton(this));
    ui->comboBox->lineEdit()->setReadOnly(true);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
     //调色板
    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPointSize(20);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->lcdNumber->setFont(font);
}
void Readnum::init()
{
    ui->lineEdit_number->setText("");
    ui->radioButton->setChecked(true);
    ui->pushButton_Ok_3->setStyleSheet("border-image: url(:/image/picture/qietu/06+.png);");
    ui->pushButton_Ok_4->setStyleSheet("border-image: url(:/image/picture/qietu/06-.png);");
    ui->lineEdit_number->setFocus();
    ui->comboBox->lineEdit()->setText("请选择您的小区");
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    startTimer();
}
Readnum::~Readnum()
{
    delete ui;
}
/*返回按钮*/
void Readnum::on_pushButton_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_number->clear();
    disconnecslots();
    socket->effect->begin(this,OperateFile::ui_heaterhomepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
    emit heaterhomgage_back();
}
/**/
void Readnum::readCard_reshow()
{
  connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
  init();
}
void Readnum::replyFinished(QNetworkReply *reply)
{
    messagebox->closeDialog();
    if(reply->error() == QNetworkReply::NoError)
     {
         QStringList m_list;
         char m_receivedata[1024];
         memset(m_receivedata,0,1024);
         reply->read(m_receivedata,1024);
         //qDebug()<<QString::fromLocal8Bit(m_receivedata);
         OperateFile::tracelog("Http received:\t"+QString::fromLocal8Bit(m_receivedata));
         m_HttpData=QString::fromLocal8Bit(m_receivedata).split("@");
         if(m_HttpData.size()<2)
         {
             OperateFile::tracelog("in readnum"+QString(INTERFACEPARAMETERERROR));
             return;
         }
         m_list=QString(m_HttpData.at(1)).split("!");
         int StrType=QString(m_list.at(0)).toInt();
         ui->lineEdit->clear();
         ui->lineEdit_2->clear();
         ui->lineEdit_number->clear();
         //qDebug()<<m_HttpData.size();
         switch (StrType) {
         case 1:
             if(m_HttpData.size()!=22){
                 OperateFile::tracelog("in readnum"+QString("interface error"));
                 return;}
             socket->m_heatersearchmsg={ m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),
                                         m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14), m_HttpData.at(15),
                                         m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21)};
             if(socket->Isheaterpurchase==true)
              {
                 disconnecslots();
                 socket->effect->begin(this,OperateFile::ui_heatermsgconfirm,LEFTTORIGHT,NONE,HIDE);
                 OperateFile::ui_heatermsgconfirm->init();
              }
              else{

                  disconnecslots();
                  socket->effect->begin(this,OperateFile::ui_heaterkhmx,LEFTTORIGHT,NONE,HIDE);
                  OperateFile::ui_heaterkhmx->init();
                }
             break;
         case 2:
                if(btnGroup->checkedId()==0){
                    QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                    messagebox->displayWithoutButton(this,"门牌号输入有误");}
                else if(btnGroup->checkedId()==1){
                    QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                    messagebox->displayWithoutButton(this,"房屋编号输入有误");}
             break;
         case 3:
             if(m_HttpData.size()!=22){
                 OperateFile::tracelog("in readnum"+QString("interface error"));
                 return;}
             socket->m_heatersearchmsg={m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),
                                        m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14), m_HttpData.at(15),
                                        m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21)};
             if(socket->Isheaterpurchase==true)
              {
                 if(socket->m_heatersearchmsg.qjzcnf=="0"){
                 QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
                 messagebox->displayWithoutButton(this,QString("您在%1缴费周期未算费").arg(m_list.at(1)));}
                 else{
                     disconnecslots();
                     socket->effect->begin(this,OperateFile::ui_heatermsgconfirm,LEFTTORIGHT,NONE,HIDE);
                     OperateFile::ui_heatermsgconfirm->init();
                    }
              }
              else
             {

                 disconnecslots();
                 socket->effect->begin(this,OperateFile::ui_heaterkhmx,LEFTTORIGHT,NONE,HIDE);
                 OperateFile::ui_heaterkhmx->init();
             }
                break;
         case 4:
             if(m_HttpData.size()!=22){
                 OperateFile::tracelog("in readnum"+QString("interface error"));
                 return;}
             socket->m_heatersearchmsg={m_HttpData.at(2),m_HttpData.at(3),m_HttpData.at(4),m_HttpData.at(5),m_HttpData.at(6),m_HttpData.at(7),m_HttpData.at(8),
                                                         m_HttpData.at(9),m_HttpData.at(10),m_HttpData.at(11),m_HttpData.at(12),m_HttpData.at(13),m_HttpData.at(14), m_HttpData.at(15),
                                                         m_HttpData.at(16),m_HttpData.at(17),m_HttpData.at(18),m_HttpData.at(19),m_HttpData.at(20),m_HttpData.at(21)};
             if(socket->Isheaterpurchase==true)
              {
                 if(socket->m_heatersearchmsg.qjzcnf=="0"){
                 QTimer::singleShot(3000, messagebox, SLOT(closeDialog()));
                 messagebox->displayWithoutButton(this,QString("您在%1周期费用已交清").arg(m_list.at(1)));}
                 else{
                     disconnecslots();
                     socket->effect->begin(this,OperateFile::ui_heatermsgconfirm,LEFTTORIGHT,NONE,HIDE);
                     OperateFile::ui_heatermsgconfirm->init();
                 }
              }
              else
             {
                 disconnecslots();
                 socket->effect->begin(this,OperateFile::ui_heaterkhmx,LEFTTORIGHT,NONE,HIDE);
                 OperateFile::ui_heaterkhmx->init();
             }
             break;
         default:
             break;
         }

      }
    else
    {
        OperateFile::tracelog("server refuse");
        QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
        messagebox->displayWithoutButton(this,"服务暂不可用，请到柜台办理业务");
        on_pushButton_back_homepage_clicked();
    }
    reply->deleteLater();
}
/*返回首页*/
void Readnum::on_pushButton_back_homepage_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_number->clear();
    emit stop();
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
}
void Readnum::on_pushButton_22_clicked()
{
     mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"1");
}

void Readnum::on_pushButton_23_clicked()
{
     mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"2");
}

void Readnum::on_pushButton_20_clicked()
{
        mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"3");
}

void Readnum::on_pushButton_21_clicked()
{
    mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"4");
}

void Readnum::on_pushButton_29_clicked()
{
    mousePressEvent(NULL);
    focuslineEdit->setText(focuslineEdit->text()+"5");
}

void Readnum::on_pushButton_25_clicked()
{
     mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"6");
}

void Readnum::on_pushButton_26_clicked()
{
    mousePressEvent(NULL);
    focuslineEdit->setText(focuslineEdit->text()+"7");
}

void Readnum::on_pushButton_24_clicked()
{
    mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"8");
}

void Readnum::on_pushButton_27_clicked()
{
    mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"9");
}

void Readnum::on_pushButton_28_clicked()
{
    mousePressEvent(NULL);
     focuslineEdit->setText(focuslineEdit->text()+"0");
}
void Readnum::on_pushButton_2_clicked()
{
    mousePressEvent(NULL);
    focuslineEdit->setText(focuslineEdit->text()+"00");
}

void Readnum::on_pushButton_BackSapce_3_clicked()
{
    mousePressEvent(NULL);
    if(focuslineEdit->text().isEmpty())
        return;
   QString text=focuslineEdit->text();
   text.chop(1);
   focuslineEdit->setText(text);

}

void Readnum::on_pushButton_Reset_3_clicked()
{
    mousePressEvent(NULL);
    if(focuslineEdit->text().isEmpty())
        return;
    focuslineEdit->text().clear();
    focuslineEdit->setText("");
}

void Readnum::on_pushButton_Ok_3_clicked()
{
   QString string;
   QString xqbh;
   QString sjbh=socket->qybh_qymc_list.key("奎屯市");
   QStringList xqbh_list=socket->qybh_xqbh_list.values(socket->qybh_qymc_list.key("奎屯市"));
   foreach (QString xqbh_string, xqbh_list)
   {
      if(socket->xqbh_xqmc_list.value(xqbh_string)==ui->comboBox->currentText())
      {
            xqbh=xqbh_string;
      }
   }
   OperateFile::readiniFile("heater.ini","interface/SEARCH",string);
   switch (btnGroup->checkedId()) {
   case 0:   // 小区编号查询方式
       if(ui->comboBox->currentText()=="请选择您的小区")
       {
           QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"请选择您的小区");
           break;
       }
        if(ui->lineEdit->text()==""||ui->lineEdit_2->text()=="")
        {
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"请输入门牌号");
            break;
        }
        ui->lineEdit_number->clear();
        messagebox->displayWithoutBlock(this,"正在查询……");
        socket->HttpSend(QUrl(QString(string).arg("").arg(xqbh).arg(ui->lineEdit->text()+"-"+ui->lineEdit_2->text()).arg(sjbh)));
        OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg("").arg(xqbh).arg(ui->lineEdit->text()+"-"+ui->lineEdit_2->text()).arg(sjbh));
       break;
   case 1:   // 房屋编号查询方式
       if(ui->lineEdit_number->text()=="")
       {
           QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
           messagebox->displayWithoutButton(this,"请输入房屋编号");
           break;
       }
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        messagebox->displayWithoutBlock(this,"正在查询……");
        socket->HttpSend(QUrl(QString(string).arg(ui->lineEdit_number->text()).arg("").arg("").arg("")));
        OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(ui->lineEdit_number->text()).arg("").arg("").arg(""));
       break;
   default:
       break;
   }
}
void Readnum::on_pushButton_Ok_4_clicked()
{
    QString string;
    QString xqbh;
    QString sjbh=socket->qybh_qymc_list.key("奎屯市");
    QStringList xqbh_list=socket->qybh_xqbh_list.values(socket->qybh_qymc_list.key("奎屯市"));
    foreach(QString xqbh_string, xqbh_list)
    {
       if(socket->xqbh_xqmc_list.value(xqbh_string)==ui->comboBox->currentText())
       {
             xqbh=xqbh_string;
       }
    }
    OperateFile::readiniFile("heater.ini","interface/SEARCH",string);
    switch (btnGroup->checkedId()) {
    case 0:   // 小区编号查询方式
        if(ui->comboBox->currentText()=="请选择您的小区")
        {
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"请选择您的小区");
            break;
        }
         if(ui->lineEdit->text()==""||ui->lineEdit_2->text()=="")
         {
             QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
             messagebox->displayWithoutButton(this,"请输入门牌号");//提示未输入小区编号和门牌号//提示未输入小区编号和门牌号
             break;
         }
         ui->lineEdit_number->clear();
         messagebox->displayWithoutBlock(this,"正在查询……");
         socket->HttpSend(QUrl(QString(string).arg("").arg(xqbh).arg(ui->lineEdit->text()+"-"+ui->lineEdit_2->text()).arg(sjbh)));
         OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg("").arg(xqbh).arg(ui->lineEdit->text()+"-"+ui->lineEdit_2->text()).arg(sjbh));
        break;
    case 1:   // 房屋编号查询方式
        if(ui->lineEdit_number->text()=="")
        {
            QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
            messagebox->displayWithoutButton(this,"请输入房屋编号");
            break;
        }
         ui->lineEdit->clear();
         ui->lineEdit_2->clear();
         messagebox->displayWithoutBlock(this,"正在查询……");
         socket->HttpSend(QUrl(QString(string).arg(ui->lineEdit_number->text()).arg("").arg("").arg("")));
         OperateFile::tracelog(QString("Send jsp:\t")+QString(string).arg(ui->lineEdit_number->text()).arg("").arg("").arg(""));
        break;
    default:
        break;
    }
}
void Readnum::waitTimeout()
{
    emit updateDispaly();
    ui->frame_lcd->show();
}
void Readnum::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_currenttime=SEC;
    ui->lcdNumber->display(m_currenttime);
    ui->frame_lcd->hide();
    emit start(MSEC);
}
void Readnum::startTimer()
{
    m_currenttime=SEC;
    ui->lcdNumber->display(SEC);
    ui->frame_lcd->hide();
    emit start(MSEC);
}
void Readnum::setLcdnum()
{

    if(ui->lcdNumber->value()==0)
    {

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_number->clear();
        disconnecslots();
        socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE,WAIYE);
    }
    else
    {
        m_currenttime--;
        ui->lcdNumber->display(m_currenttime);
    }
}
bool Readnum::eventFilter(QObject *watched, QEvent *event)
{
     if (watched==ui->lineEdit_number)         //首先判断控件(这里指 lineEdit1)
     {
          if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
          {
              ui->radioButton_2->setChecked(true);
              focuslineEdit=ui->lineEdit_number;
              ui->pushButton_Ok_3->setStyleSheet("border-image: url(:/image/picture/qietu/06+.png);");
              ui->pushButton_Ok_4->setStyleSheet("border-image: url(:/image/picture/qietu/06-.png);");
          }
          else if (event->type()==QEvent::FocusOut)    // 这里指 lineEdit1 控件的失去焦点事件
          {

          }
     }
    else if (watched==ui->lineEdit_2)           //这里来处理 lineEdit2 , 和处理lineEdit1 是一样的
     {
          if (event->type()==QEvent::FocusIn)
         {
            ui->radioButton->setChecked(true);
            focuslineEdit=ui->lineEdit_2;
            ui->pushButton_Ok_4->setStyleSheet("border-image: url(:/image/picture/qietu/06+.png);");
            ui->pushButton_Ok_3->setStyleSheet("border-image: url(:/image/picture/qietu/06-.png);");
         }
         else if (event->type()==QEvent::FocusOut)
         {

         }
     }
     else if(watched==ui->lineEdit)
     {
         if (event->type()==QEvent::FocusIn)
        {
           ui->radioButton->setChecked(true);
           focuslineEdit=ui->lineEdit;
           ui->pushButton_Ok_4->setStyleSheet("border-image: url(:/image/picture/qietu/06+.png);");
           ui->pushButton_Ok_3->setStyleSheet("border-image: url(:/image/picture/qietu/06-.png);");
        }

     }
 return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框*/
}

void Readnum::on_pushButton_3_clicked()
{
    on_pushButton_Ok_3_clicked();

}

void Readnum::on_comboBox_currentTextChanged(const QString &arg1)
{
    //qDebug()<<"ww:"<<ui->comboBox->currentText();
    //qDebug()<<arg1;
}
void Readnum::disconnecslots()
{
    emit stop();
    disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
    disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
    disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
    disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
    disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
