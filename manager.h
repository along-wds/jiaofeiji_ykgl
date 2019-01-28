#ifndef MANAGER_H
#define MANAGER_H
#include<QObject>
#include<QTimer>
#include<commondialog.h>
class Manager:public QObject
{
    Q_OBJECT
public:
    Manager();
    ~Manager();
    commonDialog *messagebox;
private:
    QTimer *timer;

protected:
   // void timerEvent(QTimerEvent *event);
signals:
    void timeout();
    void timeout2();
public slots:
     void starttimer(long msec);
     void starttimer2();
     void onTimerout();
     void onTimerout2();
     void stoptimer();
};

#endif // MANAGER_H
