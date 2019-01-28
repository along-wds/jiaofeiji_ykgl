#ifndef WRITECARD_H
#define WRITECARD_H

#include"clientsocket.h"
#include <QWidget>
#include"operatefile.h"
#include<QtNetwork>
extern ClientSocket *socket;
namespace Ui {
class Writecard;
}

class Writecard : public QWidget
{
    Q_OBJECT

public:
    explicit Writecard(QWidget *parent = 0);
    ~Writecard();
    void HttpSend_write(const QUrl &);
    int checkCard(QString &);
    int checkCard(QString &data1, QString &data2,QString &data3,QString &data4);
    void startTimer();
    void init();
    void disconnectSlots();
signals:
      void purchase_back();
      void start(long msec);
      void restart(long msec);
      void stop();
      void updateDispaly();
private slots:
      void writecard_GetData();
      void writecard_reshow();
      void replyFinished(QNetworkReply* reply);
      //void replyFinished_writecard(QNetworkReply* reply);
      //void replyFinished_getList(QNetworkReply* reply);
      void on_pushButton_readcard_clicked();
      void on_pushButton_back_clicked();
      void waitTimeout();
      void setLcdnum();
      void on_pushButton_backHomePage_2_clicked();

      void on_pushButton_clicked();

      void on_pushButton_2_clicked();

      void on_pushButton_3_clicked();

      void on_pushButton_back_2_clicked();

      void connectError(QAbstractSocket::SocketError socketError);

private:
    Ui::Writecard *ui;
    int CardType;
    int times;//判断进入次数
    int write_times;
    QString FileData1;
    QString FileData2;
    QString FileData3;
    QString FileData4;
    Cardbase unwrite_msg;
    Fkcard fkcardmsg;
    FKBKZT fkbuztmsg;
    WRitecard writecard_msg;
    QStringList m_HttpData;
    int  m_currenttime;
    bool Timeoutflag;
private:
    void mousePressEvent(QMouseEvent *event);
};

#endif // WRITECARD_H
