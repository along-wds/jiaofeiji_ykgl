#ifndef CASHCONFIRM_H
#define CASHCONFIRM_H

#include <QWidget>
#include "common/commonwidget.h"

namespace Ui {
class cashconfirm;
}

class cashconfirm : public CommonWidget
{
    Q_OBJECT

public:
    explicit cashconfirm(QWidget *parent = 0);
    ~cashconfirm();

private:
    Ui::cashconfirm *ui;
};

#endif // CASHCONFIRM_H
