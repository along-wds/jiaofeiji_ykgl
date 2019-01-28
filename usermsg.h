#ifndef USERMSG
#define USERMSG
#include<QString>
class Cardbase
{
 public:
 QString FileDate1;
 QString FileDate2;
 QString FileDate3;
 QString FileDate4;
 QString szCardNum;
 QString szRandFromCard;
 QString szFileApp;
 QString szFileReply;
 QString szMoney;
 QString szFileParam;
};
class Lkcard
{
 public:
 QString RtnCode;
 QString CardState;
 QString ickid;
 QString ls_czcs;
 QString ls_dlzt;
 QString ls_dbzt;
 QString ls_meter_id;
 QString jlbbh;
 QString yhdabh;
};
class Fkcard
{
public:
    QString cardtype;
    QString gdcs1;
    QString ickid;
    QString khbh;
    QString dbbh;
    QString kczcs;
    QString yhdabh;
    QString retdata1;
    QString retdata2;
};
struct paystr_in
{
    char amount[12];
    char number[30];
};
struct umsstr_out
{
    char strResp[1024];
};

class voidstr_in
{
public:
    char amount[12];	/*金额（12字节，无小数点，左补0，单位：分）*/
    char expr[4];		/*有效期(4字节) */
    char old_batch[6];	/*批次号（6字节）*/
    char old_trace[6];	/*流水号 (6字节，左对齐)*/
    char old_ref[12];	/*系统参考号*/
};
typedef int(*REGIST)(int,char*);
typedef int(*INITDEV)();
typedef int(*GETSTATE)();
typedef int(*READCARD)(char*);
typedef int(*CLOSEDEV)(int);
typedef int(*OPENFEYBOARD)();
typedef uchar(*INPUTPWD)();
typedef int(*CLEARPWD)();
typedef int (*CLOSEKEYBOARD)();
typedef int (*DEAL)(paystr_in *,umsstr_out*);
typedef int (*CANCEL)(voidstr_in *);
class Usermessage
{
public:
    QString xgdj1;
    QString yhdabh;
    QString yhmc;
    QString ydhy;
    QString bank;
    QString bankaccount;
    QString path;
    QString copierid;
    QString urgeid;
    QString accountid;
    QString gsbh;
    QString fgsbh;
    QString yyzbh;
    QString Address;
    QString phone;
    QString sffs;
    QString zhye;
    QString wyj;
    QString cancharge;
    QString yhzmbh;
    QString yhzmmc;
    QString sh;
    QString fhbz;
    QString fffs;
    QString sfznum;
    QString jlxh;
    QString bjlx;
    QString syfs;
    QString ydhymc;
    QString gsmc;
    QString fgsmc;
    QString yyzmc;
    QString total;
    QString xgdj;
    QString khbz;
    QString yzje;
    QString xzje;
    QString ickye;
    QString ickid;
    QString count;
    QString jxjf;
    QString jlbbh;
    QString jeqybz;
    QString ickxzje;
    QString zmxzje;
    QString fpqybz;
    QString maxje;
    QString dllsh;
};
class Payout
{
public:
        QString rescode;
        QString bank_code;
        QString card_no;
        QString expr;
        QString batch;
        QString trace;
        QString amount;
        QString resp_chin;
        QString smchtid;
        QString stermid;
        QString reference;
        QString sdate;
        QString stime;
        QString info;
};
class   YLmsg{
public:
    QString sjwyj;
    QString sjje;
    QString bcye;
    QString fpqybz;
    QString kczje;
    QString tzje;
    QString fffs;
    QString bcsjje;
    QString bcgdje;
    QString pdbh;
} ;
class FKwithcard{
public:
    QString jfje;
    QString czje;
    QString czcs;
    QString ccs1;
    QString ccs2;
    QString ccs3;
    QString szRandFromHsm;
    QString szMac1;
    QString szMac2;
    QString szMac3;
    QString szMac4;
    QString szDes1;
    QString szDes2;
    QString szKeyData;
};
class LKwithcard{
public:
    QString gdl;
    QString xkdl;
    QString gdcs;
    QString ccs1;
};
class FKdatacheck
{
public:
    QString bcgdje;
    QString bcgdye;
    QString fpqybz;
};
class LKdatacheck
{
public:
    QString bcgdje;
    QString bcgdl;
    QString bcgdye;
    QString fpqybz;
};
class WRitecard
{
public:
    QString yyzbh;
    QString yhmc;
    QString yhdabh;
    QString sfznum;
    QString address;
    QString phone;
    QString ydhymc;

};
class  FKBKZT{
public:
    QString zhbl;
    QString djmc;
    QString dbxb;
    QString djdm;
    QString yffid;
    QString result;
    };
class LKBKZT{
public:
    QString num;
    QString je;
};
class SEARCH{
public:
    QString gsbh;
    QString fgsbh;
    QString yyzbh;
    QString cblxbh;
    QString yhdabh;
    QString yhmc;
    QString addr;
    QString tel;
    QString fffs;
    QString ftel;
    QString lxr;
    QString zjhm;
};
class HEatersearchmsg{
public:
    QString fwbh;
    QString yhmc;
    QString grzmc;
    QString xqmc;
    QString room_no;
    QString fwdz;
    QString fxzq;
    QString qjzcnf;
    QString qjwyj;
    QString zjhm;       //身份证号
    QString gsrjg;
    QString gnjg;
    QString yhlxmc;//  用户类型
    QString qjsrf;
    QString qjcnf;
    QString phone;
    QString scye;
    QString srmj;
    QString gsrs;
    QString sjmc;
    //QString gnzq;
};
class Login{
public:
    QString rybh;
    QString rymc;
    QString gsbh;
    QString fgsbh;
    QString yyzbh;
    QString sfsjsfjg;
    QString ip;
};

enum DeviceError
{
    DEALTIMEOUT,
    JSPBROKEN,
    NETWORKERROR,
    READCARDBROKEN,
    PINERROR,
    NETWORKTIMEOUT
};
enum INTERFACETYPE
{
    LOGIN=1,
    GETCZCS,
    GETYHYABH,
    GETDETAILMSG,
    CASHPAY,
    FKWRITECARD,
    LKWRITECARD,
    FKDATASEARCH,
    LKDATASEARCH,
    UNIONPAY,
    GETFKBKZT,
    GETLKBKZT,
    GETFKBKLB,
    GETLKBKLB,
    GETPRINTDATA,
    GETKHXX,
    GETKHMX,
    GETYDMX,
    DEALLOGINSERT,
    DEALLOGUPDATE,
    GGFW_ZLB,
    GGFW_EJLB,
    GGFW_NR,
    GETSXLKSTR,
    GETSXFKSTR,
    GETZXSTR

};
//银联初始化
#define INITNOERROR 0
#define INITERROR 1
#define ENTERCARDERROR 2
#define EJECTCARDERROR 3
#define CHECKCARDERROR 4
#define READCARDERROR  5
#define ELECTRICINI "INTERFACE.DATA"

#define  INTERFACEPARAMETERERROR  "Interface Return Parameter Error"
#define  NETWORKERROR "Network Connection Refused "
#define  DEVICEBROKEN   "设备故障"
#define MAX_BUFFERLEN				2096		//最大的收发包长度
#define MAX_FILE_SIZE				(10 * 1024 * 1024)

#define LOGAPPSERVER				"..//log//%s.log"
#define PPSERVERINI					"..//ini//sppserver.ini"

//msgid define
#define  HEARTBEAT	        5
#define  STARTCASH			1
#define  GETMONEY			3
#define  STOPCASH           4
#define  PRINTSLIP			6
#define  PRINTSTATE			7
#define  SOCKETMSG			9
#define  GOBACK				10
#define  Evt_SocketClosed	8
#define  GETPRINTERSTATE    11
#define  GETBILLTATE		12
#define  GETSYSTIME         13
#define  COMETYPE			14
#define  GETEPPSTATE		15
#define  GETIDCSTATE		16
#define  SIGNIN				17
#define  STARTCASHBANK		18
#define  GETBANKCARDINF		19
#define  GETPINPSW			20
#define  INPUTENCRYPT		21
#define  BANKCHARGMAC		22
#define  STARTSECURITY		23
#define	 BANKCHARGBACK		24
#define  ROLLBACK			25
#define  BACKROLLBACK		26
#define  BACKBANKCARD		27
#define	 CANCELACCEPT		28
//error code define
#define  BASE_Error_TimeOut			 0x002
#define	 BASE_Error_NoMemory		 0x001
#define	 BASE_Error_ECTF_System		 0x008
#define  BASE_Error_QueueEmpty		 0x010
#define  BASE_TIMERINTERVAL2			200          // 200 ms


//
#define WM_POSTMSGTOWEB		(WM_USER + 200)
#define WM_SENDPINGTOWEB    (WM_USER + 201)
#define WM_SETMONEY			(WM_USER + 202)
#define WM_SETASTERISK		(WM_USER + 203)
#define WM_SETPSWDLGPROMTINF (WM_USER + 204)
#define WM_CHANGEPSWDLG	  	(WM_USER + 205)
//#define	WM_CLOSELOGINDLG	WM_USER + 202

//提示信息
#define  PROMPTINF_INPUT_ACCOUNT     "1.用数字键盘手动输入户号\n2.将电卡放入读卡区，点击读卡按钮读取卡号"
#define  PROMPTINF_CONFIRM_ACCOUNT   "1.请核对您的电卡信息，如果正确请点击继续缴费，否则点击取消"
#define	 PROMPTINF_PRINT_ACCOUNT	"1.点击写卡按钮将信息写入电卡\n2.点击打印收据作为本次交易凭证换取发票"
#define  PROMPTINF_PURCHASE_ACCOUNT "1.请点击继续缴费按钮完成交易"
#define  PROMPTINF_WRITECARD_FIRST   "1.请插入您的电卡，点击读卡按钮进行补写电卡操作。\n2.点击取消按钮取消本次操作。"
#define  PROMPTINF_WRITECARD_SECOND   "1.请确认您的电卡信息点击补卡按钮进行补写电卡操作。\n2.点击取消按钮取消本次操作。"
#define  PROMPTINF_WRITECARD_THIRD   "1.点击写卡按钮将当前未写入电卡的购电信息写入您的电卡。\n2.选中您要补写的缴费购电记录，点击补卡按钮完成补写操作。"
#define  PROMPTINF_QUERY_INF "1.请核对您要查询的信息点击上一页或首页返回。\n"

//错误码
//打印机
#define  ERROR_PRINT_OPENSERIALPORT_OK      1000    //打开串口返回成功
#define  ERROR_PRINT_OPENSERIALPORT_ERROR   1001    //串口打开失败
#define  ERROR_PRINT_NOPAPER				 1002    // 没纸了
#define  ERROR_PRINT_NOPAPER_CLOSETO		 1003    // 纸将尽 不处理，但上报
#define	 ERROR_PRINT_UPNOSE		         1004    // 机头抬起的时候
#define  ERROR_PRINT_CODEDIALING			 1005     // 拨码错误
#define	 ERROR_PRINT_ONPRINT				 1006      //打印时错误
#define	 ERROR_PRINT_CUTPAPER				1007      //打印完后切纸错误
#define	 ERROR_PRINT_READSTATE				1008      //读取状态时错误
#define	 ERROR_PRINT_OFFLINE				1009      //打印机脱机状态
#define  ERROR_PRINT_CALLMODULERROR			1010	//ll模块调用失败
//银联卡读卡器

#define  ERROR_CARD_UNKNOWPORT 2000 //未知端口
#define  ERROR_CARD_OPENPORT   2001 //打开端口失败

//收钞机
//开始收钞
#define ERROR_CASH_BEGIN_ERROR 1000 //开始收钞失败
//结束收钞
#define ERROR_CASH_END_ERROR   1000 //结束收钞失败 对应bill_end_error_1000
//获取钱
#define ERROR_CASH_ACCEPT_TIMEOUT   1003 //检测钱超时
#define ERROR_CASH_ACCEPT_CMD		1002 //命令出错
#define ERROR_CASH_ACCEPT_INTOBOX   1001  //进入钞箱失败
#define ERROR_CASH_ACCEPT_ERROECMD  2 //命令不正确
#define ERROR_CASH_ACCEPT_REJECT	3  //拒钞、、
#define ERROR_CASH_ACCEPT_INTOBOXEX   4  //进入钞箱失败
#define ERROR_CASH_ACCEPT_INTOBOXEX1 	5 //进入钞箱失败
#define ERROR_CASH_ACCEPT_EXIT			6 //从缓冲中退出
#define ERROR_CASH_ACCEPT_OTHER			7 //其他错误

//重置
#define ERROR_RESET_ERROR	1000 //重置失败，对应bill_reset_error_1000
//状态
#define  ERROR_BILLSTATE_OFLINE  1 //离线状态对应bill_billstate_error_0000
#define  ERROR_BILLSTATE_FULLBOX  4	//钞箱满了
#define  ERROR_BILLSTATE_OPENBOX  5//钞箱被打开bill_billstate_error_0005
#define  ERROR_BILLSTATE_CUTOFF	  6//识钞机被堵bill_billstate_error_0006
#define  ERROR_BILLSTATE_UPNOSE	  7266//验钞机盖头掀起的状态下

#define  CASH_ACCEPT_CMDERROR 12
#define  CASH_ACCEPT_INTOBOX 13
#define  CASH_ACCEPT_REJECT 14
#define  CASH_ACCEPT_EXIT 15
#define  CASH_ACCEPT_OTHER 16
#define  CASH_END_ERRO 17
#define  CASH_END_ERRO_UNKNOW 18
#define  CASH_BEGIN_ERROR 19
#define  CASH_BEGIN_ERROR_UNKNOW 20
#define  PRINT_OPENSERIALPORT_ERROR 21
#define  PRINT_NOPAPER 22
#define  PRINT_NOPAPER_CLOSETO 23
#define  PRINT_UPNOSE 24
#define  PRINT_CODEDIALING 25
#define  PRINT_ONPRINT 26
#define  PRINT_CUTPAPER 27
#define  PRINT_READSTATE 28
#define  PRINT_OFFLINE 29
#define  PRINT_CALLMODULERROR 30
#define  PRINT_UNKNOW 31

#endif // USERMSG

