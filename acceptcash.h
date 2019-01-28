#ifndef ACCEPTCASH_H
#define ACCEPTCASH_H
#include <QWidget>
namespace Ui {
class acceptcash;
}

class acceptcash : public QWidget
{
    Q_OBJECT

public:
    explicit acceptcash(QWidget *parent = 0);
    ~acceptcash();

private:
    Ui::acceptcash *ui;
};

#endif // ACCEPTCASH_H
