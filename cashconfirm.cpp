#include "cashconfirm.h"
#include "ui_cashconfirm.h"

cashconfirm::cashconfirm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cashconfirm)
{
    ui->setupUi(this);
}
cashconfirm::~cashconfirm()
{
    delete ui;
}
