#include "acceptcash.h"
#include "ui_acceptcash.h"

acceptcash::acceptcash(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::acceptcash)
{
    ui->setupUi(this);
}
acceptcash::~acceptcash()
{
    delete ui;
}
