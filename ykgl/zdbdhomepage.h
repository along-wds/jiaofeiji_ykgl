#ifndef ZDBDHOMEPAGE_H
#define ZDBDHOMEPAGE_H

#include <QWidget>

namespace Ui {
class Zdbdhomepage;
}

class Zdbdhomepage : public QWidget
{
    Q_OBJECT

public:
    explicit Zdbdhomepage(QWidget *parent = 0);
    ~Zdbdhomepage();

private:
    Ui::Zdbdhomepage *ui;
};

#endif // ZDBDHOMEPAGE_H
