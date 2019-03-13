#include "jmsh1.h"
#include "ui_jmsh1.h"
#include "jmsh4.h"
#include "operatefile.h"
#include "homepage.h"
#include "jmsh2.h"
#include "zdbdhomepage.h"
Jmsh1::Jmsh1(QWidget *parent) :CommonWidget(0,0,parent),
    ui(new Ui::Jmsh1)
{
    ui->setupUi(this);
    Timer_Label=ui->label_14;
    Timer_LcdNumber=ui->lcdNumber;
    ui->frame_2->setObjectName("frame2");
    ui->frame_5->setObjectName("frame5");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/ykgl/select1.png);}");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->pushButton_func1->setStyleSheet("QPushButton{border:0px;}");
    ui->pushButton_func2->setStyleSheet("QPushButton{border:0px;}");
    ui->pushButton_func3->setStyleSheet("QPushButton{border:0px;}");
    ui->pushButton_end->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/end.png);}"
                                      "QPushButton:pressed{border-image: url(:/image/picture/ykgl/end.png);}");
    ui->pushButton_next->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/next.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/next.png);}");
    ui->pushButton_home->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/home.png);}"
                                                 "QPushButton:pressed{border-image: url(:/image/picture/qietu/home+.png);}");
    ui->pushButton_purchase->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/purchase.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/picture/qietu/purchase+.png);}");
    ui->pushButton_search->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/search.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/search+.png);}");
    ui->pushButton_public->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/public.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/public+.png);}");
    ui->pushButton_zdbd->setStyleSheet("QPushButton{border-image: url(:/image/picture/qietu/zdbd.png);}"
                                    "QPushButton:pressed{border-image: url(:/image/picture/qietu/zdbd+.png);}");
}

Jmsh1::~Jmsh1()
{
    delete ui;
}
void Jmsh1::init()
{
    connect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)),Qt::UniqueConnection);
    startTimer();
    on_pushButton_func1_clicked();
}

void Jmsh1::on_pushButton_back_HomePage_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_zdbdhomepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_zdbdhomepage->init();
}

void Jmsh1::on_pushButton_func1_clicked()
{
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/ykgl/select1.png);}");
    ui->pushButton_func1->setStyleSheet("QPushButton{border:0px;color:white;}");
    ui->pushButton_func2->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    ui->pushButton_func3->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    QList<QPushButton*> btns=ui->frame->findChildren<QPushButton*>();
    foreach(QPushButton * btn,btns)
    {
        delete btn;
    }
    QList<QBoxLayout*>layouts=ui->frame->findChildren<QBoxLayout*>();
    foreach(QBoxLayout * layout,layouts)
    {
        delete layout;
    }
    QHBoxLayout *horizontalLayout = new QHBoxLayout(ui->frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *btn1=new QPushButton(ui->frame);
    btn1->setObjectName(QStringLiteral("pushButton"));
    btn1->setMinimumSize(QSize(178, 235));
    btn1->setMaximumSize(QSize(178, 235));
    horizontalLayout->addWidget(btn1);
    btn1->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-1.png);}");

    QPushButton *btn2=new QPushButton(ui->frame);
    btn2->setObjectName(QStringLiteral("pushButton_2"));
    btn2->setMinimumSize(QSize(178, 235));
    btn2->setMaximumSize(QSize(178, 235));
    horizontalLayout->addWidget(btn2);
    btn2->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-2.png);}");

    connect(btn2,SIGNAL(clicked()),this,SLOT(openScan()));
    connect(btn1,SIGNAL(clicked()),this,SLOT(openScan()));
}

void Jmsh1::on_pushButton_func2_clicked()
{
    ui->pushButton_func2->setStyleSheet("QPushButton{border:0px;color:white;}");
    ui->pushButton_func1->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    ui->pushButton_func3->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/ykgl/select2.png);}");
    QList<QPushButton*> btns=ui->frame->findChildren<QPushButton*>();
    foreach(QPushButton * btn,btns)
    {
        delete btn;
    }
    QList<QBoxLayout*>layouts=ui->frame->findChildren<QBoxLayout*>();
    foreach(QBoxLayout * layout,layouts)
    {
        delete layout;
    }
    QHBoxLayout *horizontalLayout = new QHBoxLayout(ui->frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *btn1=new QPushButton(ui->frame);
    btn1->setObjectName(QStringLiteral("pushButton"));
    btn1->setMinimumSize(QSize(178, 235));
    btn1->setMaximumSize(QSize(178, 235));
    btn1->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-1.png);}");
    horizontalLayout->addWidget(btn1);

    QPushButton *btn2=new QPushButton(ui->frame);
    btn2->setObjectName(QStringLiteral("pushButton_2"));
    btn2->setMinimumSize(QSize(178, 235));
    btn2->setMaximumSize(QSize(178, 235));
    btn2->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-2.png);}");
    horizontalLayout->addWidget(btn2);

    QPushButton *btn3=new QPushButton(ui->frame);
    btn3->setObjectName(QStringLiteral("pushButton_3"));
    btn3->setMinimumSize(QSize(178, 235));
    btn3->setMaximumSize(QSize(178, 235));
    btn3->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-3.png);}");
    horizontalLayout->addWidget(btn3);

    QPushButton *btn4=new QPushButton(ui->frame);
    btn4->setObjectName(QStringLiteral("pushButton_4"));
    btn4->setMinimumSize(QSize(178, 235));
    btn4->setMaximumSize(QSize(178, 235));
    horizontalLayout->addWidget(btn4);
    btn4->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-4.png);}");
    connect(btn1,SIGNAL(clicked()),this,SLOT(openScan()));
}
void Jmsh1::openScan()
{
    socket->HttpSend(QUrl("http://122.113.5.215:6060/dlfw/rest/api/bdzn/queryXzqy?sjywbh="));
    qDebug()<<"hhhhhhhhhh";
}

void Jmsh1::on_pushButton_func3_clicked()
{
    ui->pushButton_func3->setStyleSheet("QPushButton{border:0px;color:white;}");
    ui->pushButton_func1->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    ui->pushButton_func2->setStyleSheet("QPushButton{border:0px;color:#333333;}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/ykgl/select3.png);}");
    QList<QPushButton*> btns=ui->frame->findChildren<QPushButton*>();
    foreach(QPushButton * btn,btns)
    {
        delete btn;
    }
    QList<QBoxLayout*>layouts=ui->frame->findChildren<QBoxLayout*>();
    foreach(QBoxLayout * layout,layouts)
    {
        delete layout;
    }
    QHBoxLayout *horizontalLayout = new QHBoxLayout(ui->frame);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *btn1=new QPushButton(ui->frame);
    btn1->setObjectName(QStringLiteral("pushButton"));
    btn1->setMinimumSize(QSize(178, 235));
    btn1->setMaximumSize(QSize(178, 235));
    btn1->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-1.png);}");
    horizontalLayout->addWidget(btn1);

    QPushButton *btn2=new QPushButton(ui->frame);
    btn2->setObjectName(QStringLiteral("pushButton_2"));
    btn2->setMinimumSize(QSize(178, 235));
    btn2->setMaximumSize(QSize(178, 235));
    btn2->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-2.png);}");
    horizontalLayout->addWidget(btn2);

    QPushButton *btn3=new QPushButton(ui->frame);
    btn3->setObjectName(QStringLiteral("pushButton_3"));
    btn3->setMinimumSize(QSize(178, 235));
    btn3->setMaximumSize(QSize(178, 235));
    btn3->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-3.png);}");
    horizontalLayout->addWidget(btn3);

    QPushButton *btn4=new QPushButton(ui->frame);
    btn4->setObjectName(QStringLiteral("pushButton_4"));
    btn4->setMinimumSize(QSize(178, 235));
    btn4->setMaximumSize(QSize(178, 235));
    horizontalLayout->addWidget(btn4);
    btn4->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-4.png);}");

    QPushButton *btn5=new QPushButton(ui->frame);
    btn5->setObjectName(QStringLiteral("pushButton_5"));
    btn5->setMinimumSize(QSize(178, 235));
    btn5->setMaximumSize(QSize(178, 235));
    horizontalLayout->addWidget(btn5);
    btn5->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/1-5.png);}");
    connect(btn1,SIGNAL(clicked()),this,SLOT(openScan()));
}

void Jmsh1::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
         QVector<QStringList>context;
         QString retcode;
         QStringList param;
         param<<"xmbh"<<"xmmc";
         QByteArray receiveData=reply->readAll();
         OperateFile::getJson(receiveData,retcode,context,param);
         //OperateFile::tracelog("Http received:\t"+QString(receiveData));
         foreach (QStringList list, context) {
            qDebug()<<"111:"<<list.at(0)<<list.at(1);
         }
    }
}

void Jmsh1::on_pushButton_next_clicked()
{
    disconnectSlots();
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    OperateFile::ui_jmsh2=new Jmsh2();
    OperateFile::ui_jmsh2->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this,OperateFile::ui_jmsh2,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_jmsh2->init();
}

void Jmsh1::on_pushButton_end_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_zdbdhomepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_zdbdhomepage->init();
}

void Jmsh1::on_pushButton_home_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->init();
}

void Jmsh1::on_pushButton_purchase_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->IsPurchase=true;
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    OperateFile::ui_readcard->init();
}

void Jmsh1::on_pushButton_zdbd_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_zdbdhomepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_zdbdhomepage->init();
}

void Jmsh1::on_pushButton_public_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_webpage,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_webpage->init();
}

void Jmsh1::on_pushButton_search_clicked()
{
    disconnect(socket->accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    disconnectSlots();
    socket->IsPurchase=false;
    if(OperateFile::ui_readcard==0)
    {
        OperateFile::ui_readcard=new readcard();
    }
    OperateFile::ui_readcard->init();
}
