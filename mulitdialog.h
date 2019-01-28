#ifndef MULITDIALOG_H
#define MULITDIALOG_H

#include <QDialog>
#include"src/qlightboxwidget.h"
#include"qmovie.h"
namespace Ui {
class MulitDialog;
}

class MulitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MulitDialog(QWidget *parent = 0);
    ~MulitDialog();
    void Display(QWidget *parent,const QString &m_string);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MulitDialog *ui;
    QLightBoxWidget* lightBoxwithnone;
    QMovie *movie;
};

#endif // MULITDIALOG_H
