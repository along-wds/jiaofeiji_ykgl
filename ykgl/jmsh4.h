#ifndef JMSH4_H
#define JMSH4_H

#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class Jmsh4;
}

class Jmsh4 : public CommonWidget
{
    Q_OBJECT

public:
    explicit Jmsh4(QWidget *parent = 0);
    ~Jmsh4();
    void init();
private:
    Ui::Jmsh4 *ui;
};

#endif // JMSH4_H
