#ifndef READCARD_H
#define READCARD_H
#include"clientsocket.h"
#include <QWidget>
#include"operatefile.h"
#include "src/timertask.h"
#include<QtNetwork>
#include<QTimer>
namespace Ui {
class readcard;
}

class readcard : public QWidget
{
    Q_OBJECT

public:
  readcard(QWidget *parent = 0);
    ~readcard();
   int checkCard(QString &ickarr);
   int checkCard(QString&, QString&, QString &, QString &);
  void waitInsertcard();
  void startTimer();
  void init();
  void disconnectslots();
signals:
    void purchase_back();
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();
private slots:
    void setLcdnum();
    void on_pushButton_clicked();
    void on_pushButton_readCard_clicked();
    void readCard_GetData();
    void readCard_DealData();
    void readCard_reshow();
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

    void on_pushButton_Ok_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();
    void getCardtype();
    void waitTimeout();
    void on_pushButton_00_clicked();

    //void replyFinished_detailMsg(QNetworkReply* reply);

    void on_lineEdit_textChanged(const QString &arg1);
    void connectError(QAbstractSocket::SocketError socketError);
    void loopPicture();
    void setPictureOpacity();//设置透明度与位移效果
    void startSwitch();
    void dealReadCardTimeout();
private:
    int m_purchasetype;
    Ui::readcard *ui;
    int CardType;
    QString FileData1;
    QString FileData2;
    QString FileData3;
    QString FileData4;
    bool fkid;
    bool lkid;
    int Replytimes;
    bool readcard_button;
    bool msgready;

    QStringList imagesList;
    QTimer *timer;
    int m_currenttime;
    int m_currentImageindex;
    int m_nextImageindex;
    bool Timeoutflag;
    QTimer *Check_Readcard_timer;
    QTimer    *tm_start;
    QTimer    *tm_switch;
    QMovie    *mv_media;//多媒体播放
    long      switchmsec;//示意图切换定时器切换间隔(毫秒数)
    QGraphicsOpacityEffect *pictureeffect;//示意图透明度样式
    double    opacity;//透明度值
    double    opacityinterval;//淡入淡出透明度变化间隔
    int      switchcase;//淡入淡出中间状态标志
    QString CurrentCode;
public:
   QStringList m_HttpData;
   Usermessage displaymsg;
protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // READCARD_H
