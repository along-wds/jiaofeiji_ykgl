#include "jmsh2.h"
#include "ui_jmsh2.h"

Jmsh2::Jmsh2(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Jmsh2)
{
    ui->setupUi(this);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}

Jmsh2::~Jmsh2()
{
    delete ui;
}
