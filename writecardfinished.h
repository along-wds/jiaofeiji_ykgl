#ifndef WRITECARDFINISHED_H
#define WRITECARDFINISHED_H

#include <QWidget>

namespace Ui {
class WritecardFinished;
}

class WritecardFinished : public QWidget
{
    Q_OBJECT

public:
    explicit WritecardFinished(QWidget *parent = 0);
    ~WritecardFinished();

private:
    Ui::WritecardFinished *ui;
    int  m_currenttime;
signals:
   void back_HomePage();
   void start(long msec);
   void stop();
   void updateDispaly();
private slots:
   void waitTimeout();
   void setLcdnum();
   void on_pushButton_2_clicked();

   void on_pushButton_back_2_clicked();

   void on_pushButton_backHomePage_2_clicked();

   void on_pushButton_clicked();

   void on_pushButton_3_clicked();

   void on_pushButton_4_clicked();

public:
     void display();
     void init();
     void startTimer();
     void disconnectSlots();
protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // WRITECARDFINISHED_H
