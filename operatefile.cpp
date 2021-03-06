#include "operatefile.h"
#include"qmutex.h"
#include"qdebug.h"
#include "input2018/frminput2018.h"
OperateFile::OperateFile()
{

}

void OperateFile::readiniFile(const QString &filename, const QString &indexName, QString& asReslut)
{
    QSettings *configIniRead = new QSettings(filename, QSettings::IniFormat);
    configIniRead->setIniCodec("GBK");
    asReslut = configIniRead->value(indexName).toString();
    delete configIniRead;
    configIniRead=NULL;
}
void OperateFile::writeiniFile(const QString &filename, const QString &indexName, const QString &asData)
{
    QSettings *configIniWrite = new QSettings(filename, QSettings::IniFormat);
    //configIniWrite->beginGroup("Option");
    configIniWrite->setValue(indexName, asData);
    delete configIniWrite;
    configIniWrite=NULL;
}
bool OperateFile::deleteFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.exists() && file.remove())
          return true;
    return false;
}
void OperateFile::tracelog(const QString &asData)
{
    static QMutex mutex;
    mutex.lock();
    QString logDate=QDateTime::currentDateTime().toString("yyyy_MM_dd");
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QDir dir;
    if(!dir.exists("log"))
        dir.mkdir("log");
    QString filePath = "log/"+logDate + ".log";
    QFile file(filePath);
    QTextStream out(&file);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
        std::cerr << qPrintable(file.errorString());
    out<<"["<<time<<"]"<<"\t"<<asData<<"\n";
    file.flush();
    file.close();
    mutex.unlock();
}
void OperateFile::deletefile(const QString &path)
{
    //判断路径是否存在
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }
    QString logDate =  QDateTime::currentDateTime().toString("yyyy_MM");
    for(int i=0;i<file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString m_filepath = file_info.filePath();
        if(file_info.created().addMonths(2).toString("yyyy_MM")<=logDate)
        {
            deleteFile(m_filepath);
        }

    }
}
void OperateFile::getXmlVersion(const QString &filename, QString &version)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QXmlStreamReader xmlReader(&file);
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name()=="Version")
            {

               version=xmlReader.readElementText();

            }
        }
    }
}
void OperateFile::getUpdateFileList(const QString &filename, QList<UpdateFile> &filelist)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QXmlStreamReader xmlReader(&file);
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name()=="File")
            {
                UpdateFile update_file;
                QXmlStreamAttributes attributes = xmlReader.attributes();
                if(attributes.hasAttribute("name"))
                {
                    update_file.name=attributes.value("name").toString();
                }
                 if(attributes.hasAttribute("path"))
                {
                   update_file.path=attributes.value("path").toString();
                }
                 if(attributes.hasAttribute("last_update_time"))
                {
                   update_file.last_update_time=attributes.value("last_update_time").toString();
                }
                 if(attributes.hasAttribute("version"))
                {
                   update_file.version=attributes.value("version").toString();
                }
                filelist.append(update_file);
            }

        }
        else if(xmlReader.isEndElement() && xmlReader.name() =="UpdateInfo")

        {


        }

    }
    file.close();
}
//拷贝文件
bool OperateFile::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("//","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir createfile;
    bool exist = createfile.exists(toDir);
    if (exist)
    {
        if(coverFileIfExist){
            createfile.remove(toDir);
        }
    }
    else
    {
        /*如果是文件目录不存在*/
        if(!createfile.exists(toDir.mid(0,toDir.lastIndexOf('/'))))
        {
            mkMutiDir(toDir.mid(0,toDir.lastIndexOf('/')));
        }
    }
    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

//拷贝文件夹
bool OperateFile::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    //toDir.replace("//","/");
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        mkMutiDir(targetDir.absolutePath());

    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName()),coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}
QString OperateFile::mkMutiDir(const QString path)
{
    QDir dir(path);
    if ( dir.exists(path)){
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if ( !dirname.isEmpty() )
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;

}
void OperateFile::getJson(QByteArray &value,QString &retcode,QVector<QStringList>& context,QStringList &titlelist)
{
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(value,&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
    {
        return;
    }
    QJsonObject jsonObject = document.object();
    retcode=jsonObject["retCode"].toString();
    if(jsonObject.contains("resp"))
    {

        QJsonValue arrayValue = jsonObject.value(QStringLiteral("resp"));
        if(arrayValue.isArray())
        {
            QStringList list;
            QJsonArray array = arrayValue.toArray();
            for(int i=0;i<array.size();i++)
    {
                list.clear();
                QJsonValue resultArray = array.at(i);
                QJsonObject result = resultArray.toObject();
                foreach(QString title,titlelist)
                {
                    //list<<result[title].toString();
                    list.append(result[title].toString());

                }
                context.append(list);
            }
        }
    }

}
void OperateFile::hidePanle()
{
    frmInput2018::Instance()->hide();
}
void OperateFile::getIdentifyCode(QString& code,int len)
{
    code.clear();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0; i<len; i++)
    {
        code.append(QString::number(qrand()%10));
    }

}
