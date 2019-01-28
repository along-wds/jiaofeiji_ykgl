#include "devlist.h"
#include "ui_devlist.h"

Devlist::Devlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Devlist)
{
    ui->setupUi(this);
}
void Devlist::setList()
{
  ui->tableWidget->setRowCount(row);    //设置行数为10
  ui->tableWidget->setColumnCount(6);   //设置列数为6
  ui->tableWidget->setWindowTitle("QTableWidget & Item");
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  QStringList header;
  header<<"设备"<<"状态";
  QFont font = ui->tableWidget->horizontalHeader()->font();
  font.setBold(true);
  ui->tableWidget->horizontalHeader()->setFont(font);
  ui->tableWidget->setHorizontalHeaderLabels(header);
  ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列表头
  ui->tableWidget->show();
}
Devlist::~Devlist()
{
    delete ui;
}
