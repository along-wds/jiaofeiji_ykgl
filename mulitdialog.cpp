#include "mulitdialog.h"
#include "ui_mulitdialog.h"
#include<QFont>
#include<QPalette>
#include<QDesktopWidget>
MulitDialog::MulitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MulitDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label_3->setAlignment(Qt::AlignCenter);
    //取消对话框标题
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //取消对话框标题和边框
    this->setAutoFillBackground(true);
    QFont ft;
    ft.setPointSize(20);
    ui->label_3->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->label_3->setPalette(pa);
   // lightBoxwithnone=NULL;
    //movie = new QMovie(":/image/picture/qietu/电能卡.jpg");
    //ui->label->setMovie(movie);
}
MulitDialog::~MulitDialog()
{
    delete ui;
}
void MulitDialog::Display(QWidget *parent, const QString &m_string)
{
    //movie->start();
    lightBoxwithnone = new QLightBoxWidget(parent);
    ui->label_3->setText(m_string);
    lightBoxwithnone->show();
    ui->pushButton->show();
    this->exec();
}
void MulitDialog::on_pushButton_clicked()
{
    if(lightBoxwithnone!=NULL)
    {
        lightBoxwithnone->close();
        lightBoxwithnone->deleteLater();
        lightBoxwithnone=NULL;
    }
    this->close();
}
