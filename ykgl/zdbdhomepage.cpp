#include "zdbdhomepage.h"
#include "ui_zdbdhomepage.h"

Zdbdhomepage::Zdbdhomepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Zdbdhomepage)
{
    ui->setupUi(this);
}

Zdbdhomepage::~Zdbdhomepage()
{
    delete ui;
}
