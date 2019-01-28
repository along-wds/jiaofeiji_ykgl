#ifndef MESSAGESHOW_H
#define MESSAGESHOW_H

#include <QWidget>

namespace Ui {
class Messageshow;
}

class Messageshow : public QWidget
{
    Q_OBJECT

public:
    explicit Messageshow(QWidget *parent = 0);
    ~Messageshow();
private slots:
    void on_comboBox_currentTextChanged(QString);
    void on_pushButton_ConfirmPurchase_clicked();

private:
    Ui::Messageshow *ui;
};

#endif // MESSAGESHOW_H
