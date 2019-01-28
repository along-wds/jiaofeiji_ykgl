#include "update.h"
#include "ui_update.h"
#include<QFile>
#include<QMessageBox>
#include<QProcess>
#include<QDateTime>
#include<login.h>
Update::Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    nextBlockSize=0;
    bytesWritten=0;
    ui->progressBar->setMaximum(0);
        QFile vfile("version.dat");
        if (vfile.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&vfile);
            version = stream.readLine();
            vfile.close();
        }
        //获取服务器地址
        QString ipAdd;//服务器地址
        QFile netfile("net.dat");
        if (netfile.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&netfile);
            ipAdd = stream.readLine();
            netfile.close();
        }
        //连接到服务器
        clientSocket=new QTcpSocket(0);
        clientSocket->connectToHost(ipAdd,3327);
        QObject::connect(clientSocket,SIGNAL(readyRead()),this,SLOT(communicate()));
        QObject::connect(clientSocket,SIGNAL(disconnected()),this,SLOT(disconnect()));
}

Update::~Update()
{
    delete ui;
}
void Update::communicate()
{
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_4_5);
    while(1)
    {
        int bytes=(int)(clientSocket->bytesAvailable());
        //已经接收
        bytesWritten+=bytes;
        if(ui->progressBar->maximum())
        {
            //设置进度条当前进度
            ui->progressBar->setValue(bytes);
            //comeLabel->setText(QString::number(bytes));
        }
        if (nextBlockSize == 0)
        {
            if (quint64(bytes) < sizeof(quint64))
            {
                break;
            }
            //总的数据大小
            in >> nextBlockSize;
            //设置进度条最大值
            ui->progressBar->setMaximum((int)nextBlockSize);
            //totleLabel->setText(QString::number((int)nextBlockSize));
            this->show();
        }
        if (quint64(bytes)<nextBlockSize)
        {
            break;;
        }
        //通信协议
        quint16 protocol;
        in>>protocol;
        switch(protocol)
        {
            case 0x0001:
            {
                in>>version_s;
                //最新版本，不用升级
                if(version==version_s)
                {
                    login *w;
                    w->showFullScreen();
                }
                else
                {
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_4_5);
                    out<<quint64(0)<<quint16(0x0001);
                    out.device()->seek(0);
                    out<<quint64(block.size()-sizeof(quint64));
                    clientSocket->write(block);
                }
                break;
            }
            case 0x0002:
            {
                QByteArray block1;
                QByteArray block2;
                in>>block1>>block2;
                //打开要更新的文件并写入数据
                QFile file1("sppserver.exe");
                if (file1.open(QIODevice::WriteOnly)&&block1.length()!=0)
                {
                    file1.write(block1);
                    file1.close();
                }
                QFile file2("opion.ini");
                if (file2.open(QIODevice::WriteOnly)&&block2.length()!=0)
                {
                    file2.write(block2);
                    file2.close();
                }
                //修改版本文件
                QFile wvfile("version.dat");
                if (wvfile.open(QIODevice::WriteOnly))
                {
                    wvfile.write(version_s.toUtf8());
                    wvfile.write("/r/n");
                    //写入更新日期
                    wvfile.write(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
                    wvfile.close();
                }
                QMessageBox::information(this,tr("升级成功"),"您的软件版本已经从"+version+"升级到"+version_s+"，将重启启动软件！");
                //启动主程序
                QProcess::startDetached("MRP.exe");
                qApp->quit();
                break;
            }
        }
        nextBlockSize=0;
    }
}
void Update::disconnect()
{
    clientSocket->deleteLater();
    QMessageBox::information(this,tr("错误"),tr("网络连接中断！"));
    qApp->quit();
}
