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
    query.exec("create table t_DetailAccount(yhdabh varchar(20) not null, "
                    "yhmc varchar(20) not null,"
                    "yllsh varchar(20),"
                    "date DATE(20) not null,"
                    "jfje float,"
                    "jflx tinyint,"
                    "dealresult tinyint,"
                    "remark varchar(50))"
               );
    qDebug()<<query.lastError().text();
    qDebug()<<db.lastError().text();
}
void AccountDatabase::insertData(const QString &yhdabh, const QString &yhmc, const QString &yllsh, const QString &date, const float &jfje, const uint &jflx,const uint &dealresult,const QString &remark)
{
    QSqlQuery query;
    query.prepare("insert into t_DetailAccount values(:yhdabh,:yhmc,:yllsh,:date,:jfje,:jflx,:yldealresult,:actualresult,:remark)");
    query.bindValue(":yhdabh",yhdabh);
    query.bindValue(":yhmc",yhmc);
    query.bindValue(":yllsh",yllsh);
    query.bindValue(":date",date);
    query.bindValue(":jfje",jfje);
    query.bindValue(":jflx",jflx);
    query.bindValue(":dealresult",dealresult);
    query.bindValue(":remark",remark);
    query.exec();
    //qDebug()<<query.lastError().text();
    //qDebug()<<db.lastError().text();
    //query.exec( QString("UPDATE t_System  SET starttime=:time").arg());
}

