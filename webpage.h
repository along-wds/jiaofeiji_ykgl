#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWidget>
#include "common/commonwidget.h"
#include <ActiveQt/QAxWidget>
#include"qurl.h"
namespace Ui {
class WebPage;
}

class WebPage : public CommonWidget
{
    Q_OBJECT

public:
    explicit WebPage(QUrl, QWidget *parent = 0);
    void loadNavigate();
    void init();
    ~WebPage();
signals:
    void start(long msec);
    void restart(long msec);
    void stop();
    void updateDispaly();
private slots:
    void on_pushButton_back_homepage_clicked();
    void disconnectSlots();
    void setLcdnum();
    void startTimer();
    void waitTimeout();
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QUrl sUrl;
    Ui::WebPage *ui;
    int m_currenttime;

};

#endif // WEBPAGE_H
