#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include<QtNetwork/QTcpSocket>
namespace Ui {
class Update;
}

class Update : public QDialog
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = 0);
    ~Update();
private:
    QTcpSocket *clientSocket;
    quint64 nextBlockSize;
    QString version,version_s;
    int bytesWritten;
private slots:
    void disconnect();
    void communicate();
private:
    Ui::Update *ui;
};

#endif // UPDATE_H
