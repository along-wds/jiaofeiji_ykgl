#include "cashconfirm.h"
#include "ui_cashconfirm.h"

cashconfirm::cashconfirm(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::cashconfirm)
{
    ui->setupUi(this);
}
cashconfirm::~cashconfirm()
{
    delete ui;
}
