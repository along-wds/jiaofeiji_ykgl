#ifndef JMSH1_H
#define JMSH1_H

#include <QWidget>
#include "common/commonwidget.h"
#include<QNetworkReply>
namespace Ui {
class Jmsh1;
}

class Jmsh1 : public CommonWidget
{
    Q_OBJECT

public:
    explicit Jmsh1(QWidget *parent = 0);
    ~Jmsh1();
    void init();
private slots:

    void on_pushButton_back_HomePage_clicked();

    void on_pushButton_func1_clicked();

    void on_pushButton_func2_clicked();
    void openScan();
    void on_pushButton_func3_clicked();

    void replyFinished(QNetworkReply* reply);

    void on_pushButton_next_clicked();

    void on_pushButton_end_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_purchase_clicked();

    void on_pushButton_zdbd_clicked();

    void on_pushButton_public_clicked();

    void on_pushButton_search_clicked();

private:
    Ui::Jmsh1 *ui;
};

#endif // JMSH1_H
