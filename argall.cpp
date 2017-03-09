#include "argall.h"
#include <QDesktopServices>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>


ArgAll::ArgAll()
{
}
QString ArgAll::documentPath(){

    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).replace("/","\\");
}
QString ArgAll::configPath(){

    return  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).replace("/","\\")+"\\MediaIDEFile";

}
QString  ArgAll::readFile(QString path){

    QFile file(path);
    QString allStr="";
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return "";
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();
    allStr+=line;
    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }
    return allStr;
}

bool ArgAll::outputFile(QString path, QString content){

    QFile file(path);
    QFileInfo fi=QFileInfo(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, "提示信息：", "文件写入失败！");
        return false;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<content;
    file.close();
    return true;
}
QString ArgAll::getTemplatePath(){
    return ":/new/prefix1/Template/template.html";

}
QString ArgAll::replaceTemplate(QString temp, QStringList strList){
    temp.replace("%(title)s",strList[0]);
    temp.replace("%(author)s",strList[1]);
    temp.replace("%(content)s",strList[2]);
    temp.replace("%(date)s",strList[3]);
    return temp;
}

QStringList ArgAll::parseMJson(QString path){
    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();

//以下代码约定stringlist里面的个位含义
    QString title=json.value("title").toString();
    QString author=json.value("author").toString();
    QString date="测试数据";
    QString contentHtml=json.value("content").toString();

    QStringList strList;
    strList<<title<<author<<contentHtml<<date;
    return strList;
}

QStringList ArgAll::parsePSTJson(QString path){
    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();

    QString lastProject=json.value("lastProject").toString();

    QStringList strList;
    strList<<lastProject;
    return strList;
}

QStringList ArgAll::parseMSTJson(QString path){
    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();

    QString projectName=json.value("name").toString();

    QStringList strList;
    strList<<projectName;
    return strList;
}

bool ArgAll::modifyJson(QString path,QString key, QString value){
    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();
    json.remove(key);
    json.insert(key,value);
    QString output=QString(QJsonDocument(json).toJson());
    ArgAll::outputFile(path,output);
}




bool ArgAll::modifyPSTJson(QString key, QString value){
  ArgAll::modifyJson(ArgAll::getSettingPath(),key,value);
}



QString ArgAll::getSettingPath(){

    return QCoreApplication::applicationDirPath()+"\\userinfo.pst";

}

bool ArgAll::createFile(QString path){

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    return true;
}
