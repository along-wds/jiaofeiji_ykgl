#ifndef HEATERKHMX_H
#define HEATERKHMX_H
#include <QWidget>
#include "common/commonwidget.h"
#include<QtNetwork>
namespace Ui {
class Heaterkhmx;
}

class Heaterkhmx : public CommonWidget
{
    Q_OBJECT
signals:
    void start(long msec);
    void stop();
    void back_readnum();
     void updateDispaly();
public:
    explicit Heaterkhmx(QWidget *parent = 0);
    ~Heaterkhmx();
    void setList();
    void disPlay();
    void init();
    void startTimer();

protected:
   void mousePressEvent(QMouseEvent *event);
private slots:
    void on_pushButton_backfirst_clicked();
    void setLcdnum();
    void waitTimeout();
    void replyFinished(QNetworkReply*reply);
    void on_pushButton_ConfirmPurchase_4_clicked();

private:
    Ui::Heaterkhmx *ui;
    int row;
    QStringList m_HttpData;
     int  m_currenttime;
};

#endif // HEATERKHMX_H
