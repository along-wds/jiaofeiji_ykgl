#include "jmsh4.h"
#include "ui_jmsh4.h"
#include "clientsocket.h"
Jmsh4::Jmsh4(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Jmsh4)
{
    ui->setupUi(this);
    Timer_Label=ui->label_14;
    Timer_LcdNumber=ui->lcdNumber;
    ui->frame_2->setObjectName("frame2");
    ui->frame_2->setStyleSheet("QFrame#frame2{border-image: url(:/image/picture/qietu/20.png);}");
    ui->comboBox->setLineEdit(new QTComboBoxButton(this));
    ui->comboBox->lineEdit()->setReadOnly(true);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}

Jmsh4::~Jmsh4()
{
    delete ui;
}
void Jmsh4::init()
{
    startTimer();
    //on_pushButton_func1_clicked();
    ui->comboBox->lineEdit()->setStyleSheet("font-size:10px;color:white;");
    ui->comboBox->addItem("11111");
    ui->comboBox->addItem("22222");
    ui->comboBox->addItem("33333");
    ui->comboBox->addItem("44444");
    ui->comboBox->lineEdit()->setText("请选择您的小区");

}
