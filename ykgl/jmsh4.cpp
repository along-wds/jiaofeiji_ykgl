#include "jmsh4.h"
#include "ui_jmsh4.h"
#include "homepage.h"
#include "zdbdhomepage.h"
#include "jmsh1.h"
#include "jmsh2.h"
#include "jmsh3.h"
Jmsh4::Jmsh4(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Jmsh4)
{
    ui->setupUi(this);
    Timer_Label=ui->label_14;
    Timer_LcdNumber=ui->lcdNumber;
    ui->frame->setObjectName("frame");
    ui->frame->setStyleSheet("QFrame#frame{border-image: url(:/image/picture/ykgl/combobox.png);}");
    ui->comboBox->setLineEdit(new QTComboBoxButton(this));
    ui->comboBox->lineEdit()->setReadOnly(true);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
    ui->frame_5->setObjectName("frame5");
    ui->frame_5->setStyleSheet("QFrame#frame5{border-image: url(:/image/picture/qietu/xiadaohang.jpg);}");
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

Jmsh4::~Jmsh4()
{
    delete ui;
}
void Jmsh4::init()
{
    startTimer();
    //on_pushButton_func1_clicked();
    ui->comboBox->lineEdit()->setFocusPolicy(Qt::NoFocus);
    ui->comboBox->lineEdit()->setStyleSheet("font-size:10px;");
    ui->comboBox->addItem("11111");
    ui->comboBox->addItem("22222");
    ui->comboBox->addItem("33333");
    ui->comboBox->addItem("44444");
    for(int i=0;i<10;i++)
    ui->comboBox->addItem("5555555");
    ui->comboBox->lineEdit()->setText("请选择");
    ui->comboBox->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void Jmsh4::on_pushButton_end_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_zdbdhomepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_jmsh1->deleteLater();
    OperateFile::ui_jmsh2->deleteLater();
    OperateFile::ui_jmsh3->deleteLater();
    OperateFile::ui_zdbdhomepage->init();
}


void Jmsh4::on_pushButton_back_clicked()
{
    disconnectSlots();
    socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_jmsh1->deleteLater();
    OperateFile::ui_jmsh2->deleteLater();
    OperateFile::ui_jmsh3->deleteLater();
    OperateFile::ui_homepage->init();
}
