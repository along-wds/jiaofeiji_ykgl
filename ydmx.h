#ifndef YDMX_H
#define YDMX_H

#include <QWidget>
#include<QtNetwork>
namespace Ui {
class Ydmx;
}

class Ydmx : public QWidget
{
    Q_OBJECT

public:
    explicit Ydmx(QWidget *parent = 0);
    ~Ydmx();
     void setList();
signals:
     void back_Lookup();
     void start(long msec);
     void stop();
     void updateDispaly();
public slots:
   void replyFinished(QNetworkReply*reply);
private slots:
   void on_pushButton_clicked();
   void waitTimeout();
   void setLcdnum();
   void on_pushButton_back_clicked();
   void flushPage(int flag);
   void on_pushButton_2_clicked();

   void on_pushButton_3_clicked();
   void disconnectSlots();
   void on_pushButton_home_clicked();

   void on_pushButton_purchase_clicked();

   void on_pushButton_search_clicked();

   void on_pushButton_public_clicked();
   void setCalendarStyle();
private:
    Ui::Ydmx *ui;
    int row;
    QStringList m_HttpData;
    int m_currenttime;
    int TotalResultSum;
    int CurrentPageIndex;
    int TotalPageSum;
protected:
   void mousePressEvent(QMouseEvent *event);
private:
   void startTimer();
   void setRows();
public slots:
   bool eventFilter(QObject *, QEvent *);    //注意这里
};

#endif // YDMX_H
