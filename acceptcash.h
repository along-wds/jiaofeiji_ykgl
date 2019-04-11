#ifndef ACCEPTCASH_H
#define ACCEPTCASH_H
#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class acceptcash;
}

class acceptcash : public CommonWidget
{
    Q_OBJECT

public:
    explicit acceptcash(QWidget *parent = 0);
    ~acceptcash();

private:
    Ui::acceptcash *ui;
};

#endif // ACCEPTCASH_H
