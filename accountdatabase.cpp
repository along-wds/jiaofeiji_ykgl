#include "accountdatabase.h"
#include"qmessagebox.h"
#include"qsqlquery.h"
#include"qsqlerror.h"
#include"qmessagebox.h"
#include"qvariant.h"
#include"qdebug.h"
#include "qthread.h"
AccountDatabase::AccountDatabase()
{
    bool create = !QFile::exists("SppserverDatabase.db");
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("AccoutDatabase.db"); //数据库连接命名
    if (!db.open())
    {
        QMessageBox::warning(NULL, tr("Error"),
            tr("Can not open the sqlite database"));
        exit(2);
    }
    qDebug()<<"create:"<<QThread::currentThreadId()<<create;
    if(create)
        createTable();
}
void AccountDatabase::createTable()
{
    QSqlQuery query;
    query.exec("create table t_DetailAccount(用户号 varchar(20) not null, "
               "用户名称 varchar(20) not null,"
               "流水号 varchar(20),"
               "日期 DATE(20) not null,"
               "缴费金额 varchar(10) not null,"
               "缴费类型 varchar(2) not null,"
               "扣款结果 varchar(2),"
               "备注 varchar(50))"
          );
    qDebug()<<query.lastError().text();
    qDebug()<<db.lastError().text();
}
void AccountDatabase::insertData(const QString &yhdabh, const QString &yhmc, const QString &yllsh, const QString &date, const QString &jfje, const QString &jflx,const QString &dealresult,const QString &remark)
{
    QSqlQuery query;
    query.prepare("insert into t_DetailAccount values(:yhdabh,:yhmc,:yllsh,:date,:jfje,:jflx,:dealresult,:remark)");
    query.bindValue(":yhdabh",yhdabh);
    query.bindValue(":yhmc",yhmc);
    query.bindValue(":yllsh",yllsh);
    query.bindValue(":date",date);
    query.bindValue(":jfje",jfje);
    query.bindValue(":jflx",jflx);
    query.bindValue(":dealresult",dealresult);
    query.bindValue(":remark",remark);
    query.exec();
    qDebug()<<query.lastError().text();
    qDebug()<<db.lastError().text();
}

