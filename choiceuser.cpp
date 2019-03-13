#include "choiceuser.h"
#include "ui_choiceuser.h"
#include"homepage.h"
#include "readcard.h"
#include"qradiobutton.h"
#include"msgconfirm.h"
#include "lookup.h"
#include"qstringlist.h"
ChoiceUser::ChoiceUser(QStringList &m_httpdata, QWidget *parent) :
     QWidget(parent),
    ui(new Ui::ChoiceUser),HttpData(m_httpdata)
{
    ui->setupUi(this);
    btnGroup=new QButtonGroup(this);
    ui->frame_5->setObjectName("frame5");
    ui->frame->setObjectName("frame1");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->frame->setStyleSheet("QFrame#frame1{background-color:#dfdfdf;border:1px;border-radius:3px;}");

    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase+.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");

    ui->tableWidget->setStyleSheet("QTableView::item{border-left: 0px darkgray;border-right:0px;border-top:0px;border-bottom:1px solid lightgray;}"
                                    "QTableView::item:selected{color:#00a797;background:white;border-top:1px solid #00a797;border-bottom:1.5px solid #00a797;}");

    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(changeCheckstate(int,int,int,int)));
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(MouseTrackItem(int)));
    QPalette widgetpat=ui->tableWidget->palette();
    widgetpat.setBrush(QPalette::Base,QBrush(QColor("#dfdfdf")));
    ui->tableWidget->setPalette(widgetpat);
    ui->tableWidget->horizontalHeader()->setPalette(widgetpat);

}
void ChoiceUser::init()
{
    connect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()),Qt::QueuedConnection);
    connect(this, SIGNAL(stop()),ui_manager, SLOT(stoptimer()),Qt::QueuedConnection);
    connect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()),Qt::QueuedConnection);
    connect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()),Qt::QueuedConnection);;
    setList();
    startTimer();

}

ChoiceUser::~ChoiceUser()
{
    //ui->tableWidget->clear();
    delete ui;
}
void ChoiceUser::setList()
{

    ui->tableWidget->setColumnCount(6);   //设置列数为6
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList header;
    header<<"选择"<<"客户编号"<<"客户名称"<<"身份证号"<<"联系电话"<<"客户地址";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列表头
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-left: 0px solid darkgray;border-right:0px;border-top:0px;border-bottom:1px solid darkgray; background:#dfdfdf;}");
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(55); //设置行距
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并平分
    /***/
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    /***/
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setFrameShape(QFrame::NoFrame);

    QFont font=ui->tableWidget->horizontalHeader()->font();

    for(int i=0;i<6;i++)
    {
       QTableWidgetItem *columnHeaderItem0 = ui->tableWidget->horizontalHeaderItem(i); //获得水平方向表头的Item对象
       columnHeaderItem0->setTextColor(QColor("#474040")); //设置文字颜色
       font.setFamily("Mircosoft YaiHei UI");
       font.setBold(true);
       font.setPointSize(13);
       columnHeaderItem0->setFont(font);
    }
    ui->tableWidget->setRowCount(HttpData.size());
    QStringList temp_list=QString(HttpData.at(0)).split("@");
    QString m_string=HttpData.at(0);
    m_string=m_string.remove(0,QString(temp_list.at(0)).size()+1);
    QStringList new_list=HttpData;
    //qDebug()<<"string:"<<m_string;
    foreach (QString string, HttpData) {
       if(string.contains("您没有收取此户的权限!"))
            new_list.removeOne(string);
    }
    //qDebug()<<"httpdata:"<<new_list;
    ui->tableWidget->setRowCount(new_list.size());
    for(int i=0;i<new_list.size();i++)
    {
        QString str=new_list.at(i);
        if(i==0)
            str=m_string;
        QStringList list=str.split("@");
        if(list.size()<24)
            continue;
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

       for(int j=0;j<6;j++)
       {

           ui->tableWidget->setItem(i,1,new QTableWidgetItem(list.at(1)));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(list.at(2)));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(list.at(24)));
           ui->tableWidget->setItem(i,4,new QTableWidgetItem(list.at(14)));
           ui->tableWidget->setItem(i,5,new QTableWidgetItem(list.at(13)));

       }
       for(int j=1;j<6;j++)
       {
           ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
       }

    }
    ui->tableWidget->show();
    ui->tableWidget->setCurrentCell(0,1);

}
void ChoiceUser::changeCheckstate(int row, int column, int pre_row, int pre_column)
{
   QList<QRadioButton*> rad = ui->tableWidget->findChildren<QRadioButton *>();
   rad.at(row)->setChecked(true);
   //qDebug()<<"current checked row:"<<row;
   /*for(int i=1;i<6;i++)
   {

        ui->tableWidget->item(ui->tableWidget->currentRow(),i)->setTextColor(QColor(Qt::red));
   }*/
   startTimer();
}
void ChoiceUser::MouseTrackItem(int row)
{
   //ui->tableWidget->setStyleSheet("QTableWidget::item{selection-background-color:lightblue;}"); //选中项的颜色
    //qDebug()<<"2222:"<<row;
    ui->tableWidget->setCurrentCell(row, QItemSelectionModel::Select); //设置该行为选中项
    startTimer();
}
void ChoiceUser::mousePressEvent(QMouseEvent *event)
{
   Q_UNUSED(event);
   m_currenttime=SEC;
   ui->lcdNumber->display(m_currenttime);
   ui->label->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void ChoiceUser::startTimer()
{
   m_currenttime=SEC;
   ui->lcdNumber->display(SEC);
   ui->label->hide();
   ui->lcdNumber->hide();
   emit start(MSEC);
}
void ChoiceUser::setLcdnum()
{
   if(ui->lcdNumber->value()==0)
   {

       disconnectSlots();
       socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
       OperateFile::ui_homepage->init();
   }
   else
   {
       m_currenttime--;
       ui->lcdNumber->display(m_currenttime);
   }
}
void ChoiceUser::waitTimeout()
{
   emit updateDispaly();
   ui->lcdNumber->show();
   ui->label->show();
}
void ChoiceUser::disconnectSlots()
{
   emit stop();
   disconnect(this, SIGNAL(start(long)), ui_manager, SLOT(starttimer(long)));
   disconnect(ui_manager,SIGNAL(timeout()),this,SLOT(waitTimeout()));
   disconnect(this, SIGNAL(stop()), ui_manager, SLOT(stoptimer()));
   disconnect(this,SIGNAL(updateDispaly()),ui_manager,SLOT(starttimer2()));
   disconnect(ui_manager,SIGNAL(timeout2()),this,SLOT(setLcdnum()));
}

void ChoiceUser::on_pushButton_back_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_readcard,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_readcard->init();
}

void ChoiceUser::on_pushButton_clicked()
{

    int CurrentRow=ui->tableWidget->currentRow();
    QStringList temp_list=QString(HttpData.at(0)).split("@");
    QString m_string=HttpData.at(0);
    m_string.remove(0,QString(temp_list.at(0)).size()+1);
    QString str=HttpData.at(CurrentRow);
    if(CurrentRow==0)
        str=m_string;
    QStringList list=str.split("@");
    //qDebug()<<"222222"<<list.size();
    socket->message={list.at(0),list.at(1),list.at(2),list.at(3),list.at(4),list.at(5),list.at(6),list.at(7),list.at(8),list.at(9),list.at(10),list.at(11),list.at(12),list.at(13),list.at(14),list.at(15),
                     list.at(16),list.at(17),list.at(18),list.at(19),list.at(20),list.at(21),list.at(22),list.at(23),list.at(24),list.at(25),list.at(26),list.at(27),list.at(28),list.at(29),list.at(30),list.at(31),
                     list.at(32),list.at(33),list.at(34),list.at(35),list.at(36),list.at(37),list.at(38),list.at(39),list.at(40),list.at(41),list.at(42),list.at(43),list.at(44),list.at(45),list.at(46)};
    //qDebug()<<"333333333";
    if(OperateFile::ui_msgconfirm==0)
    {
       OperateFile::ui_msgconfirm =new MsgConfirm();
    }
    //qDebug()<<"444444444444444";
    disconnectSlots();
    if(socket->IsPurchase==true)
    {
        if(socket->message.fffs!="01" && socket->message.fffs!="03" )
        {
            if(socket->message.khbz=="0")
            {
                QTimer::singleShot(INFOTIME, messagebox, SLOT(closeDialog()));
                messagebox->displayWithoutButton(this,"您还未开户，请去柜台处理！");
                return;
            }
            else
            {
                    //页面显示调整金额和您的缴费金额应大于 显示
                    OperateFile::ui_msgconfirm->showTzje();
            }
        }
        //普通户
        else
        {
            //调整金额隐藏
            //您的缴费金额应大于  页面显示
             OperateFile::ui_msgconfirm->hideTzje();
        }
        socket->effect->begin(this, OperateFile::ui_msgconfirm,LEFTTORIGHT,NONE,CLOSE);
        OperateFile::ui_msgconfirm->init();
    }
    else if(socket->IsPurchase==false)
    {
        if(OperateFile::ui_lookup==0)
        {
            OperateFile::ui_lookup=new Lookup();
        }
       disconnectSlots();
       socket->effect->begin(this,OperateFile::ui_lookup,LEFTTORIGHT,NONE,CLOSE);
       OperateFile::ui_lookup->init();
    }
}

void ChoiceUser::on_pushButton_home_clicked()
{
    ui->pushButton_home->setEnabled(false);
    disconnectSlots();
    socket->effect->begin(this,OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void ChoiceUser::on_pushButton_search_clicked()
{

}
void ChoiceUser::on_pushButton_2_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}
