#include "heater/heaterlookup.h"
#include "ui_heaterlookup.h"
Heaterlookup::Heaterlookup(QWidget *parent) :
    CommonWidget(0,0,parent),
    ui(new Ui::Heaterlookup)
{
    ui->setupUi(this);
}

Heaterlookup::~Heaterlookup()
{
    delete ui;
}
void Heaterlookup::init()
{

}
