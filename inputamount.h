#ifndef INPUTAMOUNT_H
#define INPUTAMOUNT_H
#include"homepage.h"
#include"usermsg.h"
#include"dealfinish.h"
#include <QWidget>
#include<QtNetwork>
extern ClientSocket *socket;
namespace Ui {
class inputamount;
}

class inputamount : public QWidget
{
    Q_OBJECT

public:
    explicit inputamount(QWidget *parent = 0);
    ~inputamount();
    void readywriteCard();
    void writecard();
    void getErrormsg(int &ret, QString &retstr);
    void init();
    double round(double d, unsigned p);
    void startTimer();
    void cancelPay(const QString &Errorstr);
    void disconnectSlots();
    void backHomeBroken();
    int maxCheck();
signals:
   void back_HomePage();
   void start(long msec);
   void stop();
   void updateDispaly();
   void electricDealwithUnion(QString amount);
private:
    Ui::inputamount *ui;
    paystr_in InData;
    //umsstr_out OutData;
    QStringList m_HttpData;
    int CardType;
    dealfinish *form_dealfinish;
    uint writetimes;
    int  m_currenttime;
    QString dllsh;
    int m_recvtimes;
    QString IP;
    QString m_je;
    QString yl_result;
    QString m_errorstr;
private slots:

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

    void on_pushButton_back_homepage_clicked();

    void replyFinished(QNetworkReply* reply);
    void replyFinished_writecard(QNetworkReply* reply);

    void replyFinished_unifyplatform(QNetworkReply *);

    void inputamount_GetData();
    void waitTimeout();

    //void on_pushButton_readCard_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_00_clicked();

    void setLcdnum();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();

    int bitCheck();
    void GetSerialNumber();
    void getRespcode(QString amount);

protected:
   void mousePressEvent(QMouseEvent *event);
};

#endif // INPUTAMOUNT_H
