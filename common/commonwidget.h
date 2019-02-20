#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
class CommonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommonWidget(QLCDNumber *lcdnumber, QLabel *label,QWidget *parent = 0);

signals:
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();
public slots:
    void waitTimeout();
    void setLcdnum();
    void connect2Timer();
    void startTimer();
    void disconnectSlots();
    //void replyFinished(QNetworkReply* reply);
public:
    QLCDNumber * Timer_LcdNumber;
    QLabel *Timer_Label;
    int Currenttime;
protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // COMMONWIDGET_H
