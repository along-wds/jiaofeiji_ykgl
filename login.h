#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
#include"clientsocket.h"
#include<QtNetwork>
#include"operatefile.h"
#include"manager.h"
#include"commondialog.h"
#include<QThread>
#include"src/qlightboxwidget.h"
#include"keyboard/widgetKeyBoard.h"
#include"src/timertask.h"
#include"src/heartbeattask.h"
namespace Ui {
class login;
}
class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
    void InitDevice();
    void startWithoutPwd();
    void showKeyBoard();
signals:
    void login_HasData(KIND reply);
private slots:
    void on_pushButton_clicked();
    void dosomething();
    void login_GetData();
    void login_DealData(KIND reply);
    void replyFinished(QNetworkReply* reply);
    void replyFinished_getlist(QNetworkReply* reply);


private:
    Ui::login *ui;
    QTimer *time;
    quint16 blockSize;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
    QThread *thread ;
    QThread *timed_thread;
    QThread *heartbeat_thread;
    QLightBoxWidget* lightBox;
    widgetKeyBoard  *myKeyboard;
    uint flag;
    uint flag1;
      QString Printer_state;
      QString ElectricCard_state;
      QString PINkeyboard_state;
      QString Carddev_state;
      bool step1;
      bool step2;
      bool step3;
      Timertask *m_task;
      HeartbeatTask *m_heartbeat_task;
protected:
    void mousePressEvent(QMouseEvent *event);
public slots:
   bool eventFilter(QObject *,QEvent *);    //注意这里

};

#endif // LOGIN_H
