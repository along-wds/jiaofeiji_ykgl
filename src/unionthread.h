#ifndef UNIONTHREAD_H
#define UNIONTHREAD_H
#include <QObject>
#include<QTimer>
class Unionthread:public QObject
{
    Q_OBJECT
public:
    Unionthread();
    ~Unionthread();
signals:
    void initFinished(int );
    void heaterDealFinished(QString);
    void electricDealFinished(QString);
    void openFinished(int);
    void ejectCardFinished(int);
    void checkCardFinished(int);
public slots:
    void  initwithUnion();
    void  dealwithUnion(QString amount);
    void  OpenDev();
    void ejectCard();
   // void checkCard();
   // void checkTimer_out();
private:
    char *mid(int m,int n);
    double round(double d, unsigned p);
private:
    char *transmintstrResp;
    QTimer *timer;

};

#endif // UNIONTHREAD_H
