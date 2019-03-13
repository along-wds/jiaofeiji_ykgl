#ifndef OPERATEFILE_H
#define OPERATEFILE_H
#include <QString>
#include <iostream>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QDateTime>
#include <QDataStream>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include"QXmlStreamReader"
#include"usermsg.h"
class First;
class HomePage;
class PurChase;
class inputamount;
class inputpassword;
class UnionPay;
class MsgConfirm;
class dealfinish;
class Lookup;
class KHmx;
class Writecard;
class Messageshow;
class readcard;
class Heaterinputmaount;
class Heaterdealfinished;
class Readnum;
class Heaterhomepage;
class Heaterlookup;
class Heaterkhmx;
class Heatermsgconfirm;
class CashPay;
class ChoiceUser;
class WebPage;
class IdentifyCode;
class Jmsh1;
class Jmsh2;
class Jmsh3;
class Jmsh4;
class Zdbdhomepage;
#define COUNTERID "00000000"
#define OPERID    "00000000"
#define XIAOFEI "06"
#define CHEXIAO "06"
#define OLDTRACE "000000"
#define OLDREF "000000000000"
#define OLDAUTH "000000"
#define OLDBATCH "000000"
#define LRC "000"
#define ELECTRIC  2
#define HEATER  4
#define SFFSBH "024"
#define DEVINFO "C:/gmc/Pos.ini"
#define SERVERICEINFO "config.ini"
#define READCARDTIMEOUT 10
#define INFOTIME 3000
enum KIND
{
    LOGINSUCCESS,
    LOGINFAILL,

};
class UpdateFile{
public:
    QString name;
    QString path;
    QString last_update_time;
    QString version;
};
class OperateFile
{

public:
    OperateFile();
public:
    /**************************
    *readiniFile 读取ini配置文件
    *参数说明：
    *filename   文件的绝对路径
    *indexName  对应配置文件中索引名称 ex: UMP/IP
    *asReslut   返回结果
    ***************************/
    static void readiniFile(const QString& filename,const QString& indexName,QString& asReslut);
    /**************************
    *writeiniFile 修改ini配置文件
    *参数说明：
    *filename   文件的绝对路径
    *indexName  对应配置文件中索引名称 ex: UMP/IP
    *asData     写入内容
    ***************************/
    static void writeiniFile(const QString& filename,const QString& indexName,const QString& asData);
    /**************************
    *deleteFile 删除文件
    *参数说明：
    *fileName  要删除文件的绝对路径
    ***************************/
    static bool deleteFile(const QString& fileName);
    /**************************
    *tracelog 跟踪日志
    *参数说明：
    *asData  日志内容
    ***************************/
    static void tracelog(const QString& asData);
    /*

    */
    static void deletefile(const QString &path);
    /*拷贝文件*/
    static bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
    /*拷贝目录*/
    static bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    /*创建多级目录*/
    static void getUpdateFileList(const QString &filename, QList<UpdateFile> &filelist);
    static void getXmlVersion(const QString &filename,QString & version);
    static QString mkMutiDir(const QString path);
    static void getJson(QByteArray &value, QString &retcode, QVector<QStringList>&context, QStringList &titlelist);
    static void hidePanle();
    static void getIdentifyCode(QString& code,int len);
public:
   static HomePage *ui_homepage;
   static MsgConfirm *ui_msgconfirm;
   static UnionPay *ui_unionpay;
   static inputpassword *ui_inputpassword;
   static inputamount *ui_inputamount;
   static Writecard *ui_writecard;
   static Messageshow *ui_messageshow;
   static readcard *ui_readcard;
   static Lookup *ui_lookup;
   static PurChase *ui_purchase;
   static First *ui_first;
   static Heaterinputmaount *ui_heaterinputamount;
   static Heaterdealfinished *ui_heaterdealfinished;
   static Heaterhomepage *ui_heaterhomepage;
   static Heatermsgconfirm *ui_heatermsgconfirm;
   static Heaterlookup *ui_heaterlookup;
   static Heaterkhmx *ui_heaterkhmx;
   static Readnum *ui_readnum;
   static CashPay *ui_cashpay;
   static ChoiceUser *ui_choiceuser;
   static WebPage *ui_webpage;
   static IdentifyCode *ui_identifycode;
   static Jmsh1 *ui_jmsh1;
   static Jmsh2 *ui_jmsh2;
   static Jmsh3 *ui_jmsh3;
   static Jmsh4 *ui_jmsh4;
   static Zdbdhomepage *ui_zdbdhomepage;
};

#endif // OPERATEFILE_H
