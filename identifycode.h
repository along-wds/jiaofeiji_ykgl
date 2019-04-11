#ifndef IDENTIFYCODE_H
#define IDENTIFYCODE_H

#include <QWidget>
#include "common/commonwidget.h"
#include<QtNetwork>
namespace Ui {
class IdentifyCode;
}

class IdentifyCode : public CommonWidget
{
    Q_OBJECT

public:
    explicit IdentifyCode(QString telnumber,QString code,QWidget *parent = 0);
    ~IdentifyCode();
    void startTimer();
    void init();
    void disconnectSlots();
    inline QStringList& getUserInfo()
    {
        return m_HttpData;
    }
signals:
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();
private slots:
    void setLcdnum();
    void replyFinished(QNetworkReply* reply);
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_BackSapce_clicked();

    void on_pushButton_Reset_clicked();

    //void on_pushButton_Ok_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();
    void waitTimeout();
    void on_pushButton_00_clicked();

    void on_pushButton_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_OK_clicked();

protected:
   void mousePressEvent(QMouseEvent *event);
private:
    Ui::IdentifyCode *ui;
    int m_currenttime;
    QStringList m_HttpData;
    QString TelNumber;
    QString CurrentCode;
};

#endif // IDENTIFYCODE_H
