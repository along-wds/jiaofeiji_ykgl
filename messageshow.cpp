#include "messageshow.h"
#include "ui_messageshow.h"
#include"qdebug.h"
#include"first.h"
#include"QDesktopWidget"
extern ClientSocket *socket;
extern Manager *ui_manager;
Messageshow::Messageshow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Messageshow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
     //取消对话框标题
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
     //取消对话框标题和边框
    this->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0x0f,0x00,0x00));
    QDesktopWidget *deskdop = QApplication::desktop();
    move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2);
    QHash<QString, QStringList>::const_iterator i;
    QFont font;
    font.setFamily("Arial");
    font.setBold(true);
    font.setPointSize(13);
    /* for(i=socket->m_Hash2.constBegin(); i!=socket->m_Hash2.constEnd();i++)
     {

          ui->listWidget->addItem(i.key());
          ui->listWidget->item(0)->setFont(font);

     }*/
}
void Messageshow::on_comboBox_currentTextChanged(QString)
{

}
Messageshow::~Messageshow()
{
    delete ui;
}

void Messageshow::on_pushButton_ConfirmPurchase_clicked()
{
    this->close();
}
