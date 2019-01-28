#ifndef CLIENTSOCKET
#define CLIENTSOCKET
#include<QtNetwork/QTcpSocket>
#include<QProxyStyle>
#include"QtNetwork"
#include"usermsg.h"
#include"commondialog.h"
#include"src/effect.h"
#include"src/unionthread.h"
#include"QMediaPlayer"
#include<QLibrary>
#include"accountdatabase.h"
class ClientSocket : public QTcpSocket
{
        Q_OBJECT
private:
        char *m_receiveData;
        char *m_transmintData;
        QString sendstr;

public:
        ClientSocket(QObject *parent = 0);
        QString ToString();
        void sendMsg(const QString &str);
        void HttpSend(const QUrl &url);
        ~ClientSocket();
public slots:
        void receiveMsg();
        void hasConnected();

public:
        Payout OutData;
        Login LoginMessage;
        Usermessage message;
        Fkcard FK_msg;          //读卡后详细字段数据
        Lkcard LK_msg;
        Cardbase Card_Basemsg;
        int PurchaseType;       //缴费类型
        bool Unionpaychannel;
        int CardType;           //卡类型
        YLmsg YL_msg;           //调用银联缴费接口返回
        FKwithcard FK_writewithcard;   //调用费控有卡写卡返回
        LKwithcard LK_writewithcard;   //调用量控有卡写卡返回
        FKdatacheck FK_datacheck;
        LKdatacheck LK_datacheck;//调用量控数据查询接口返回
        QString ReadCard_str;   //读卡后返回数据
        bool IsPurchase;        //购电查询或购电
        bool Isheaterpurchase;  //热力查询或购热
        bool IscancelDeal;      //取消交易判断
        bool canwritecard;
        bool Isjumpreadcard;    //是否跳过读卡界面
        bool is_wait_http_resp; //是否在等待http响应
        QString amount_str;
        int flag;
        int msec;
        WRitecard writecard_msg;
        FKBKZT fkbkzt_msg;
        LKBKZT lkbkzt_msg;
        SEARCH client_msg;

        Effect *effect;
        QMediaPlayer *player;
        //MulitDialog *Multmessagebox;
        /***********************热力********************/
        QStringList xiaoqu_list;
        QStringList qy_list;
        QHash<QString, QString>qybh_xqbh_list;
        QHash<QString,QString>qybh_qymc_list;
        QHash<QString,QString>xqbh_xqmc_list;
        HEatersearchmsg m_heatersearchmsg;
        /*********************************************/

        /****************数据库*********************/
        AccountDatabase DataBase;
        /************************************/
        QLibrary  lib;
        QThread *thread ;
        Unionthread *unionThread;
public:
        QNetworkAccessManager *accessManager;
        QNetworkRequest request;
        QNetworkReply *reply;
        #define MSEC 10000
        #define LSEC 20000
        #define SEC 60
        #define PAYSEC 120
        #define  TIMEOUT 60
        struct paystr_in InData;
        struct umsstr_out RetData;
};
#endif // CLIENTSOCKET
