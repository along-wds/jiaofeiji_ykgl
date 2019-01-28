#include "chiocedialog.h"
#include "ui_chiocedialog.h"
#include "qdesktopwidget.h"
ChioceDialog::ChioceDialog(QWidget *parent2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChioceDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setAlignment(Qt::AlignCenter);
    //ui->label->setGeometry(QRect(0,0,this->width(),this->height()));
    //取消对话框标题
   this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //取消对话框标题和边框
   this->setAutoFillBackground(true);
   QPalette pal = palette();
   pal.setColor(QPalette::Background, QColor(0x00,0x0f,0x00,0x00));
   lightBoxwithmsg=NULL;
   lightBoxwithnone=NULL;

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

ChioceDialog::~ChioceDialog()
{
    delete ui;
}

void ChioceDialog::on_pushButton_ok_clicked()
{
    emit ok_clicked();
    if(lightBoxwithnone!=NULL)
    {
        lightBoxwithnone->close();
        lightBoxwithnone->deleteLater();
        lightBoxwithnone=NULL;
    }
    this->close();
}

void ChioceDialog::on_pushButton_cancel_clicked()
{
    emit cancel_clicked();
    if(lightBoxwithnone!=NULL)
    {
        lightBoxwithnone->close();
        lightBoxwithnone->deleteLater();
        lightBoxwithnone=NULL;
    }
    this->close();
}
void ChioceDialog::display(QWidget *parent,const QString &m_string)
{
    lightBoxwithnone = new QLightBoxWidget(parent);
    ui->label->setText(m_string);
    lightBoxwithnone->show();
    this->exec();
}
