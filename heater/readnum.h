#ifndef READNUM_H
#define READNUM_H

#include <QWidget>
#include"clientsocket.h"
#include"heatermsgconfirm.h"
#include"heaterkhmx.h"
#include<QButtonGroup>
#include<QLineEdit>
namespace Ui {
class Readnum;
}
class Readnum : public QWidget
{
    Q_OBJECT
public:
    explicit Readnum(QWidget *parent = 0);
    ~Readnum();
    void startTimer();
    void init();
    void disconnecslots();
signals:
    void heaterhomgage_back();
    void start(long msec);
    void restart(long msec);
    void stop();
     void updateDispaly();
private slots:
    void on_pushButton_clicked();

    void replyFinished(QNetworkReply* reply);

    void on_pushButton_back_homepage_clicked();
    void setLcdnum();
    void waitTimeout();
    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_BackSapce_3_clicked();

    void on_pushButton_Reset_3_clicked();

    void on_pushButton_Ok_3_clicked();
    void on_pushButton_Ok_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::Readnum *ui;
    QStringList m_HttpData;
    Heatermsgconfirm *form_heatermsgconfirm;
    Heaterkhmx *form_heaterkhmx;
    QButtonGroup *btnGroup;
    int  m_currenttime;
    bool canpurchase;
    QLineEdit  *focuslineEdit;

protected:
   void mousePressEvent(QMouseEvent *event);
public slots:
    void readCard_reshow();
    bool eventFilter(QObject *,QEvent *);    //注意这里
};
class ComboboxItem :public QWidget{

    explicit ComboboxItem(QWidget *parent = 0);
    ~ComboboxItem();

};


#endif // READNUM_H
