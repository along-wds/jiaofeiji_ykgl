#include "devlist.h"
#include "ui_devlist.h"
#include"qsqlquery.h"
#include"qsqlerror.h"
#include"qdebug.h"
#include "homepage.h"
Devlist::Devlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Devlist)
{
    ui->setupUi(this);
    ui->dateEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_end->setDateTime(QDateTime::currentDateTime());
    ui->lineEdit_yhdabh->setStyleSheet("font-size: 20px;border: 1px solid #333333;");
    ui->lineEdit_trace->setStyleSheet("font-size: 20px;border: 1px solid #333333;");
    ui->groupBox->setStyleSheet("QGroupBox {border: 1px solid #333333;color: #474040;}QGroupBox::title {color: #474040;subcontrol-origin: margin;subcontrol-position: top left;}");
    ui->groupBox_2->setStyleSheet("QGroupBox {border: 1px solid #333333;color: #474040;}QGroupBox::title {color: #474040;subcontrol-origin: margin;subcontrol-position: top left;}");
    getSum();
}

void Devlist::getSum()
{
    QSqlQuery query;
    query.exec(QString("SELECT SUM(缴费金额)FROM t_DetailAccount WHERE 缴费类型='0'"));
    qDebug()<<query.lastError().text();
    query.next();
    qDebug()<<"111111111111111:"<<query.value(0);
    ui->label_cash->setText(QString("%1").arg(query.value(0).toFloat()));
    query.exec(QString("SELECT SUM(缴费金额)FROM t_DetailAccount WHERE 缴费类型='1'"));
    qDebug()<<query.lastError().text();
    query.next();
    ui->label_union->setText(QString("%1").arg(query.value(0).toFloat()));
}
Devlist::~Devlist()
{
    delete ui;
}

void Devlist::closeEvent(QCloseEvent *event)
{
    qDebug()<<"22222222222222";
    //socket->effect->begin(this, OperateFile::ui_homepage,RIGHTTOLEFT,NONE,CLOSE);
    OperateFile::ui_homepage->showFullScreen();
    OperateFile::ui_homepage->init();
    this->deleteLater();
}

void Devlist::on_pushButton_clicked()
{
   qApp->quit();
}

void Devlist::on_pushButton_search_clicked()
{
    model.setTable("t_DetailAccount");
    model.setFilter(QString("date(日期) between '%1' and '%2'").arg(ui->dateEdit_start->text()).arg(ui->dateEdit_end->text()));
    if(!ui->lineEdit_yhdabh->text().isEmpty())
        model.setFilter(QString("用户号='%1'").arg(ui->lineEdit_yhdabh->text()));
    if(!ui->lineEdit_trace->text().isEmpty())
        model.setFilter(QString("流水号='%1'").arg(ui->lineEdit_trace->text()));
    model.select();
    ui->tableView->setModel(&model);
    ui->tableView->verticalHeader()->setDefaultSectionSize(35); //设置行距
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:lightblue;}");
    ui->tableView->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//使列完全填充并平分
    ui->tableView->resizeRowsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
