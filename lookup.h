#ifndef LOOKUP_H
#define LOOKUP_H

#include <QWidget>

namespace Ui {
class Lookup;
}

class Lookup : public QWidget
{
    Q_OBJECT

public:
    explicit Lookup(QWidget *parent = 0);
    ~Lookup();
   void  startTimer();
   void init();
   void disconnectSlots();
signals:
    void back_HomePage();
    void back_readcard();
    void start(long msec);
    void stop();
    void updateDispaly();
private slots:

    void on_pushButton_yhmx_clicked();

    void on_pushButton_gdmx_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_backhomepage_clicked();
    void Lookup_reshow();
    void waitTimeout();
    void on_pushButton_gdmx_2_clicked();
    void setLcdnum();
    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_back_MsgConfirm_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::Lookup *ui;
    int m_currenttime;

protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // LOOKUP_H
