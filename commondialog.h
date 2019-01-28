#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>
#include<QMovie>
#include<QLabel>
#include"src/qlightboxwidget.h"
#include<QDateTime>
namespace Ui {
class commonDialog;
}

class commonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit commonDialog(QWidget *parent2,QWidget *parent = 0 );
    commonDialog(const QString &m_text, QWidget *parent=0);
    commonDialog(const QString &m_text,const QString &m_filename, QWidget *parent=0);
    ~commonDialog();
    void display(QWidget *parent,const QString &m_string);
    void wait(QWidget *parent,const QString &DisplayMsg);
    void closeDisplay();
    void displayWithoutButton(QWidget *parent,const QString &DisplayMsg);
    void displayWithoutBlock(QWidget *parent,const QString &DisplayMsg);
private slots:
    void on_pushButton_clicked();
private:
    Ui::commonDialog *ui;
    QLightBoxWidget* lightBoxwithnone;
    QLightBoxWidget* lightBoxwithmsg;
private:
    QMovie *movie;
    QLabel *label;
    QString text;
    QString filename;
    QDateTime n2;
    QDateTime now;
public slots:
     void closeDialog();
private slots:
    void paintEvent(QPaintEvent *);

};
#endif // COMMONDIALOG_H
