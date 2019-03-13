#include "login.h"
#include<QLibrary>
#include <QApplication>
#include<QTextCodec>
#include"src/update.h"
#include"qsplashscreen.h"
#include "input2018/frminput2018.h"
REGIST regist;
INITDEV initdev;
GETSTATE getstate;
READCARD read;
CLOSEDEV closedev;
OPENFEYBOARD openkeyboard;
INPUTPWD inputpwd;
CLEARPWD clearpwd;
CLOSEKEYBOARD closekeyboard;
DEAL deal;
CANCEL cancelpay;
//ClientSocket *socket;
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    QString logDate=QDateTime::currentDateTime().toString("yyyy_MM_dd");
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("[Debug]");
        break;

    case QtWarningMsg:
        text = QString("[Warning]");
        break;

    case QtCriticalMsg:
        text = QString("[Critical]");
        break;

    case QtFatalMsg:
        text = QString("[Fatal]");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("[%1]:").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(current_date).arg(context_info).arg(msg);

    QDir dir;
    if(!dir.exists("log"))
        dir.mkdir("log");
    QString filePath = "log/"+logDate + "_debug.log";
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
        std::cerr << qPrintable(file.errorString());
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory shared_memory;
    shared_memory.setKey(QString(argv[0]));
    if(shared_memory.attach())
    {
        return 0;
    }

    if(shared_memory.create(1))
    {

    QSplashScreen splash(QPixmap(":/image/picture/qietu/底纹.jpg"));
    splash.setDisabled(true); //禁用用户的输入事件响应
    splash.show();
    splash.showMessage("程序启动中……", Qt::AlignCenter, Qt::white);

    /**********/

    /***********/
    frmInput2018::Instance()->hide();
    frmInput2018::Instance()->setFontInfo("Microsoft YaHei", 20, 18);
    frmInput2018::Instance()->setFrmSize(680, 280);
    frmInput2018::Instance()->setIconSize(30, 30);
    frmInput2018::Instance()->setTopHeight(45);
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QString qss;
        QFile qssFile(":/image/radiobutton.qss");

        qssFile.open(QFile::ReadOnly);

        if(qssFile.isOpen())

        {
            qss = QLatin1String(qssFile.readAll());
            qApp->setStyleSheet(qss);
            qssFile.close();

        }
    //qInstallMessageHandler(outputMessage);
    QLibrary lib("C:/gmc/posinf.dll");
    if(lib.load())
    {
        qDebug()<<"load success";
        OperateFile::tracelog("load posinf.dll success");
    }
    regist=(REGIST)lib.resolve("APPS_Login");
    initdev=(INITDEV)lib.resolve("UMS_OpenCard");
    getstate=(GETSTATE)lib.resolve("UMS_CheckCard");
    read=(READCARD)lib.resolve("UMS_ReadCard");
    closedev=(CLOSEDEV)lib.resolve("UMS_Close");
    openkeyboard=(OPENFEYBOARD)lib.resolve("PIN_Open");
    inputpwd=(INPUTPWD)lib.resolve("PIN_ReadOneByte");
    clearpwd=(CLEARPWD)lib.resolve("PIN_GetPinValue");
    closekeyboard=(CLOSEKEYBOARD)lib.resolve("PIN_Destroy");
    deal=(DEAL)lib.resolve("UMS_Pay_Dll");
    cancelpay=(CANCEL)lib.resolve("UMS_Void_Dll");
    //socket=new ClientSocket();
    qDebug()<<"argc"<<argc;
    login w;
    int delayTime = 3;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         a.processEvents();
    }
    //splash.finish(&w);
    if(argc>1)
    {
       //socket->LoginMessage.fgsbh=QString(argv[1]);
       //socket->LoginMessage.gsbh=QString(argv[2]);
       //socket->LoginMessage.rybh=QString(argv[3]);
       //socket->LoginMessage.rymc=QString(argv[4]);
       //socket->LoginMessage.sfsjsfjg=QString(argv[5]);
       //socket->LoginMessage.yyzbh=QString(argv[6]);
       w.startWithoutPwd();
    }
    else
    {
      //w.startWithoutPwd();
      //w.showKeyBoard();
      QPalette palette1;
      w.showFullScreen();
      palette1.setBrush(w.backgroundRole(),QBrush(QPixmap(":/image/picture/qietu/底纹.jpg").scaled(w.size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
      w.setPalette(palette1);
    }
    splash.finish(&w);
    return a.exec();
    }
}
