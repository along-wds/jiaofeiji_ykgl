#include "zdbdhomepage.h"
#include "ui_zdbdhomepage.h"
#include "homepage.h"
#include "jmsh1.h"
Zdbdhomepage::Zdbdhomepage(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Zdbdhomepage)
{
    ui->setupUi(this);
    Timer_Label=ui->label_6;
    Timer_LcdNumber=ui->lcdNumber;
    ui->frame_2->setObjectName("frame2");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->pushButton_jmsh->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/jmsh.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/jmsh.png);}");
    ui->pushButton_qygy->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/qygy.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/qygy.png);}");
    ui->pushButton_dyfjm->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/dyfjm.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/dyfjm.png);}");
    ui->pushButton_jtbz->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/jtbz.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/jtbz.png);}");
    ui->pushButton_ls->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/ls.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/ls.png);}");
    ui->pushButton_zr->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/zr.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/zr.png);}");
    ui->pushButton_gm->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/gm.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/gm.png);}");
    ui->pushButton_jr->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/jr.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/jr.png);}");
    ui->pushButton_xh->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/xh.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/xh.png);}");
    ui->pushButton_zt->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/zt.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/zt.png);}");
    ui->pushButton_dyzjr->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/zjr.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/zjr.png);}");
    ui->pushButton_jdcx->setStyleSheet("QPushButton{border-image: url(:/image/picture/ykgl/jd.png);}"
                                       "QPushButton:pressed{border-image: url(:/image/picture/ykgl/jd.png);}");
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

Zdbdhomepage::~Zdbdhomepage()
{
    delete ui;
}
void Zdbdhomepage::init()
{
    startTimer();
}

void Zdbdhomepage::on_pushButton_back_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,HIDE);
    OperateFile::ui_homepage->init();
}

void Zdbdhomepage::on_pushButton_jmsh_clicked()
{
    disconnectSlots();

    OperateFile::ui_jmsh1=new Jmsh1();
    OperateFile::ui_jmsh1->setAttribute(Qt::WA_DeleteOnClose);
    socket->effect->begin(this,OperateFile::ui_jmsh1,LEFTTORIGHT,NONE,HIDE);
    OperateFile::ui_jmsh1->init();
}
