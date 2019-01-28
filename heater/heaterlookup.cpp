#include "heater/heaterlookup.h"
#include "ui_heaterlookup.h"
Heaterlookup::Heaterlookup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Heaterlookup)
{
    ui->setupUi(this);
}

Heaterlookup::~Heaterlookup()
{
    delete ui;
}
