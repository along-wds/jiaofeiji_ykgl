#ifndef INPUTPASSWORD_H
#define INPUTPASSWORD_H
#include"homepage.h"
#include"usermsg.h"
#include <QWidget>
#include "common/commonwidget.h"
#include<QTimer>
#include<QDate>
extern ClientSocket *socket;
namespace Ui {
class inputpassword;
}

class inputpassword : public CommonWidget
{
    Q_OBJECT
public:
    explicit inputpassword(QWidget *parent = 0);
    ~inputpassword();
    void startTimer();
    void init();
    void InputPwd();
    void CheckPwd();
    void ClosekeyBoard();
    void beginInput();
    void backFirstPage();
    void disconnectSlots();
    void cancelDeal();
signals:
   void back_HomePage();
   void start(long msec);
   void stop();
   void inputnum();
   void updateDispaly();
   void back_unionpay();
   void openKeyBoard();
public slots:
   //void setLcdnum();
   void on_pushButton_back_clicked();
   void waitTimeout();
   void OpenkeyBoard();
   void setLcdnum();
   void on_pushButton_home_clicked();

   void on_pushButton_purchase_clicked();

   void on_pushButton_search_clicked();

   void on_pushButton_public_clicked();
private:
    Ui::inputpassword *ui;
    QTimer *timer;
    QTimer *timer2;
    int m_currenttime;
    QElapsedTimer sleeptime;
    uchar key_out;
    QString lineEdit_text;
    bool m_waitTimeout;
    bool Isputtonbusy;
protected:
   void mousePressEvent(QMouseEvent *event);
};
#endif // INPUTPASSWORD_H
