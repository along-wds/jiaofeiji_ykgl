#ifndef ACCOUNTDATABASE_H
#define ACCOUNTDATABASE_H
#include <QObject>
#include<QSqlDatabase>
//#include <QtSql/qtsqlglobal.h>
#include"qsql.h"
#include"qfile.h"
class AccountDatabase:public QObject
{
private:
    QSqlDatabase db;
    void errorSQLOrder(QSqlQuery query, QString mark);
    void createTable();
public:
    AccountDatabase();
    void initiDatabase();
    void insertData(const QString &yhdabh, const QString &yhmc, const QString &yllsh, const QString &date, const float &jfje, const uint &jflx, const uint &dealresult, const QString &remark);
    void deleteData();
};
#endif // ACCOUNTDATABASE_H
