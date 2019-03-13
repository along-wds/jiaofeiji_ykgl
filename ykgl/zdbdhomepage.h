#ifndef ZDBDHOMEPAGE_H
#define ZDBDHOMEPAGE_H

#include <QWidget>
#include "common/commonwidget.h"
namespace Ui {
class Zdbdhomepage;
}

class Zdbdhomepage : public CommonWidget
{
    Q_OBJECT

public:
    explicit Zdbdhomepage(QWidget *parent = 0);
    void init();
    ~Zdbdhomepage();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_jmsh_clicked();

private:
    Ui::Zdbdhomepage *ui;
};

#endif // ZDBDHOMEPAGE_H
