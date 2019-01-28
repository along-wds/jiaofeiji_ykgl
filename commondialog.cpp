#include "commondialog.h"
#include "ui_commondialog.h"
#include<QFont>
#include<QPalette>
#include<QDesktopWidget>
#include<QGridLayout>
#include<QPainter>
#include<QBitmap>
commonDialog::commonDialog(QWidget *parent2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::commonDialog)
{
    ui->setupUi(this);
    movie = new QMovie(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setGeometry(QRect(0,0,this->width(),this->height()));
    //取消对话框标题
   this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //取消对话框标题和边框
   this->setAutoFillBackground(true);
   QPalette pal = palette();
   pal.setColor(QPalette::Background, /*QColor(0x00,0x0f,0x00,0x00)*/Qt::white);
   //this->setPalette(pal);
   lightBoxwithmsg=NULL;
   lightBoxwithnone=NULL;
   QDesktopWidget *deskdop = QApplication::desktop();
   /*处理多显示器分辨率问题*/
   QDesktopWidget *deskTop = QApplication::desktop();
   int curMonitor = deskTop->screenNumber(parent2); // 参数是一个QWidget*
   QRect rect = deskTop->screenGeometry(curMonitor);
   QFont ft;
   ft.setPointSize(22);
   ui->label->setFont(ft);
   //设置颜色
   QPalette pa;
   pa.setColor(QPalette::WindowText,Qt::red);
   ui->label->setPalette(pa);
   move((rect.width() - this->width())/2, (rect.height() - this->height())/2);
}
commonDialog::commonDialog(const QString &m_text, QWidget *parent)
{

}
commonDialog::~commonDialog()
{
    delete ui;
    delete movie;
}
void commonDialog::display(QWidget *parent,const QString &m_string)
{
    lightBoxwithnone = new QLightBoxWidget(parent);
    ui->label->setText(m_string);
    lightBoxwithnone->show();
    ui->pushButton->show();
    this->exec();
}
void commonDialog::wait(QWidget *parent,const QString &DisplayMsg)
{
    lightBoxwithmsg = new QLightBoxWidget(parent);
    QLabel* lbProgress = new QLabel;
    QMovie* progressMovie = new QMovie(":/image/image/loader.gif");
    lbProgress->setMovie(progressMovie);
    progressMovie->start();
    QLabel* lbDescription = new QLabel(DisplayMsg);
    lbDescription->setStyleSheet("color: white");
    QGridLayout* lbLayout = new QGridLayout;
    lbLayout->setRowStretch(0, 1);
    lbLayout->setColumnStretch(0, 1);
    lbLayout->addWidget(lbProgress, 1, 1, Qt::AlignRight);
    lbLayout->setColumnStretch(3, 1);
    lbLayout->addWidget(lbDescription, 2, 1, 1, 2);
    lbLayout->setRowStretch(4, 1);
    lightBoxwithmsg->setLayout(lbLayout);
    lightBoxwithmsg->show();
}
void commonDialog::displayWithoutButton(QWidget *parent,const QString &DisplayMsg)
{
    lightBoxwithnone = new QLightBoxWidget(parent);
    ui->label->clear();
    ui->label->setText(DisplayMsg);
    lightBoxwithnone->show();
    ui->pushButton->hide();
    this->exec();
}
void commonDialog::displayWithoutBlock(QWidget *parent, const QString &DisplayMsg)
{
    lightBoxwithnone = new QLightBoxWidget(parent);
    ui->label->clear();
    ui->label->setText(DisplayMsg);
    ui->pushButton->hide();
    lightBoxwithnone->show();
    this->show();
}
void commonDialog::on_pushButton_clicked()
{
    if(lightBoxwithnone!=NULL)
    {
        lightBoxwithnone->close();
        lightBoxwithnone->deleteLater();
        lightBoxwithnone=NULL;
    }
    this->close();
}
void commonDialog::closeDisplay()
{
    if(lightBoxwithmsg!=NULL)
    {
         lightBoxwithmsg->close();
         lightBoxwithmsg->deleteLater();
         lightBoxwithmsg=NULL;
    }
}
void commonDialog::closeDialog()
{
   on_pushButton_clicked();
}
void commonDialog::paintEvent(QPaintEvent *)
{
   // QPainter p(this);
   // p.drawRoundedRect(0, 0, width() - 1, height() - 1, 20, 20);
}
