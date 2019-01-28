#ifndef CHIOCEDIALOG_H
#define CHIOCEDIALOG_H

#include <QDialog>
#include"src/qlightboxwidget.h"
namespace Ui {
class ChioceDialog;
}

class ChioceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChioceDialog(QWidget *parent2,QWidget *parent = 0);
    ~ChioceDialog();
    void display(QWidget *parent,const QString &m_string);
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ChioceDialog *ui;
    QLightBoxWidget* lightBoxwithnone;
    QLightBoxWidget* lightBoxwithmsg;
signals:
    ok_clicked();
    cancel_clicked();
};

#endif // CHIOCEDIALOG_H
