#include "jmsh3.h"
#include "ui_jmsh3.h"

Jmsh3::Jmsh3(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Jmsh3)
{
    ui->setupUi(this);
    QPalette lcdpat =ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::white);
     //设置当前窗口的调色板
    ui->lcdNumber->setPalette(lcdpat);
}

Jmsh3::~Jmsh3()
{
    delete ui;
}
