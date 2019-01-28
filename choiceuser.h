#ifndef CHOICEUSER_H
#define CHOICEUSER_H

#include <QWidget>
#include "qbuttongroup.h"
namespace Ui {
class ChoiceUser;
}

class ChoiceUser : public QWidget
{
    Q_OBJECT

public:
    void setList();
    void init();
    explicit ChoiceUser(QStringList &,QWidget *parent = 0);
    ~ChoiceUser();
signals:
    void back_readcard();
    void start(long msec);
    void stop();
    void updateDispaly();
protected:
   void mousePressEvent(QMouseEvent *event);
private slots:
   void setLcdnum();
   void waitTimeout();
   void startTimer();
   void disconnectSlots();
   void on_pushButton_back_clicked();
   void MouseTrackItem(int);
   void changeCheckstate(int,int,int,int);
   void on_pushButton_clicked();

   void on_pushButton_home_clicked();

   void on_pushButton_search_clicked();

   void on_pushButton_2_clicked();

private:
    Ui::ChoiceUser *ui;
    int m_currenttime;
    QButtonGroup *btnGroup;
    QStringList HttpData;
};

#endif // CHOICEUSER_H
