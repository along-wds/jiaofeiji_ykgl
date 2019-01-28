#ifndef KHXX_H
#define KHXX_H

#include <QWidget>
#include<QtNetwork>
namespace Ui {
class KHxx;
}

class KHxx : public QWidget
{
    Q_OBJECT

public:
    explicit KHxx(QWidget *parent = 0);
    ~KHxx();
    void display();
    void startTimer();
signals:
    void back_Lookup();
    void start(long msec);
    void stop();
    void updateDispaly();
private slots:
    void waitTimeout();
    void setLcdnum();
    void on_pushButton_back_2_clicked();
    void replyFinished(QNetworkReply*reply);
    void on_pushButton_home_clicked();
    void disconnectSlots();
    void on_pushButton_purchase_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_public_clicked();

protected:
   void mousePressEvent(QMouseEvent *event);

private:
    Ui::KHxx *ui;
    int m_currenttime;
    QStringList m_HttpData;
};

#endif // KHXX_H
