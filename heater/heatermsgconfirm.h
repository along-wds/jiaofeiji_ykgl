#ifndef HEATERMSGCONFIRM_H
#define HEATERMSGCONFIRM_H
#include <QWidget>
#include "common/commonwidget.h"
#include<QtNetwork>
namespace Ui {
class Heatermsgconfirm;
}

class Heatermsgconfirm : public CommonWidget
{
    Q_OBJECT
public:
    explicit Heatermsgconfirm(QWidget *parent = 0);
    ~Heatermsgconfirm();
    void disPlay();
     void init();
     void startTimer();
signals:
    void back_readnum();
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();

private:
    QStringList httpData;
    Ui::Heatermsgconfirm *ui;
    int  m_currenttime;
protected:
   void mousePressEvent(QMouseEvent *event);
private slots:
   void on_pushButton_back_clicked();
   void on_pushButton_ConfirmPurchase_clicked();
   void heatermsgconfirm_reshow();
   void on_pushButton_backfirst_clicked();
   void setLcdnum();
   void waitTimeout();
   void disconnectSlots();
   void replyFinished(QNetworkReply* reply);
   void on_pushButton_clicked();
   //void on_pushButton_ConfirmPurchase_2_clicked();
};

#endif // HEATERMSGCONFIRM_H
