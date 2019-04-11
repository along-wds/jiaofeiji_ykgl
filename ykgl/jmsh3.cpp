#include "jmsh3.h"
#include "ui_jmsh3.h"
#include "jmsh4.h"
#include "jmsh2.h"
#include "jmsh1.h"
#include "homepage.h"
#include "zdbdhomepage.h"
Jmsh3::Jmsh3(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Jmsh3)
{
    ui->setupUi(this);
    Timer_Label=ui->label_14;
    Timer_LcdNumber=ui->lcdNumber;
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->frame->setObjectName("frame");
    ui->frame_2->setObjectName("frame2");
    ui->frame_5->setObjectName("frame5");
    ui->frame_6->setObjectName("frame6");
    ui->frame->setStyleSheet("QFrame#frame{border-image: url(:/image/picture/ykgl/lineedit.png);}");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/ykgl/lineedit.png);}");
    ui->frame_6->setStyleSheet("QFrame#frame6{border-image: url(:/image/picture/ykgl/lineedit.png);}");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    ui->pushButton_end->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/end.png);}"
                                      "QPushButton:pressed{border-image: url(:/image/picture/ykgl/end.png);}");
    ui->pushButton_next->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/next.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/next.png);}");
    ui->pushButton_pre->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/pre.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/pre.png);}");
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

Jmsh3::~Jmsh3()
{
    delete ui;
}
void Jmsh3::init()
{
    startTimer();
}
void Jmsh3::on_pushButton_pre_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_jmsh2,RIGHTTOLEFT,NONE,CLOSE);
}

void Jmsh3::on_pushButton_next_clicked()
{
    disconnectSlots();
    OperateFile::ui_jmsh4=new Jmsh4();
    OperateFile::ui_jmsh4->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this, OperateFile::ui_jmsh4,LEFTTORIGHT,NONE,HIDE);

}

void Jmsh3::on_pushButton_end_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_zdbdhomepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_jmsh1->deleteLater();
    OperateFile::ui_jmsh2->deleteLater();
}

void Jmsh3::on_pushButton_back_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_jmsh2,RIGHTTOLEFT,NONE,CLOSE);

}
