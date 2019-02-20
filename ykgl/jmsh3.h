#ifndef JMSH3_H
#define JMSH3_H

#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class Jmsh3;
}

class Jmsh3 : public CommonWidget
{
    Q_OBJECT

public:
    explicit Jmsh3(QWidget *parent = 0);
    ~Jmsh3();

private:
    Ui::Jmsh3 *ui;
};

#endif // JMSH3_H
