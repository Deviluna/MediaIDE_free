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
#include <QDebug>
#include <QDir>
#include <QFileDialog>


ArgAll::ArgAll()
{
}
QString ArgAll::documentPath(){

    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}
QString ArgAll::configPath(){

    return  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/MediaIDEFile";

}


QString ArgAll::insertImage(QWidget *parent){
    QString path;
    QFileDialog* fileDialog = new QFileDialog(parent);
    fileDialog->setWindowTitle("插入图片");
    fileDialog->setFileMode(QFileDialog::ExistingFile);

    QStringList filters;
    filters<<"*.jpg *.png *.bmp";
    fileDialog->setNameFilters(filters);
    fileDialog->setFilter(QDir::Dirs|QDir::Files);

    if(fileDialog->exec() == QDialog::Accepted) {
        path = fileDialog->selectedFiles()[0];
    }

    return path;
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

QString ArgAll::dirProName(){
    return "属性";
}
QString ArgAll::getValueJson(QString path, QString key){

    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();
    return json.value(key).toString();

}

bool ArgAll::outputFile(QString path, QString content){

    QFileInfo fi=QFileInfo(path);


    QFile file(path);


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
    QString date=json.value("date").toString();
    QString content=json.value("content").toString();
    QString intropic=json.value("intropic").toString();
    QString introtext=json.value("introtext").toString();

    //QString mdate=json.value("mdate").toString();



    QStringList strList;
    strList<<title<<author<<content<<date<<intropic<<introtext;
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
    QString openedTab=json.value("openedTab").toString();

    QStringList strList;

    strList<<projectName<<openedTab;

    return strList;
}

bool ArgAll::modifyJson(QString path,QString key, QString value){
    QString allStr=ArgAll::readFile(path);
    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();
    json.remove(key);
    json.insert(key,value);
    QString output=QString(QJsonDocument(json).toJson());
    ArgAll::outputFile(path,output);
    return true;
}




bool ArgAll::modifyPSTJson(QString key, QString value){
    ArgAll::modifyJson(ArgAll::getSettingPath(),key,value);
    return true;
}



QString ArgAll::getSettingPath(){

    return QCoreApplication::applicationDirPath()+"/userinfo.pst";

}

bool ArgAll::createFile(QString path){

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    return true;
}

bool ArgAll::addMSTTab(QString MSTPath, QString path){
    QStringList mstList=ArgAll::parseMSTJson(MSTPath);
    QString openedTab=mstList[1];

    QStringList openedTabs=openedTab.split(",");
    for (int i=0;i<openedTabs.length();i++){
        if(openedTabs[i]==path)
            return false;
    }
    QString re="";
    if(mstList[1].length()>0)
        re+=","+path;
    else re=path;
    ArgAll::modifyJson(MSTPath,"openedTab",mstList[1]+re);

    return true;
}
bool ArgAll::removeMSTTab(QString MSTPath, int x){
    QStringList mstList=ArgAll::parseMSTJson(MSTPath);
    QStringList openedTabs=mstList[1].split(",");
    QString newTabs="";

    int first=0;
    for (int i=0;i<openedTabs.length(); i++){
        if(i==x)
            continue;
        if(first==0)
            first=1;
        else
            newTabs+=",";
        newTabs+=openedTabs[i];
    }
    ArgAll::modifyJson(MSTPath,"openedTab",newTabs);
    return true;
}

QString ArgAll::settingName(){
    return "项目设置";

}


QString ArgAll::listString(){
    QString re=ArgAll::readFile(":/new/prefix1/Template/listString.txt");
    return re;
}
QString ArgAll::replacelistString(QString temp, QStringList strList){

    temp.replace("%(address)s",strList[0]);
    temp.replace("%(title)s",strList[1]);
    temp.replace("%(introtext)s",strList[2]);
    temp.replace("%(date)s",strList[3]);

    return temp;
}

QString ArgAll::getReplacedIndex(QStringList strList,QStringList dirList){
    QString temp=ArgAll::readFile(":/new/prefix1/Template/index.html");
    temp.replace("%(lists)s",strList[0]);
    for(int i=0;i<dirList.length();i++){
        temp.replace("%(classify"+QString::number(i+1)+")s",dirList[i]);
    }

    return temp;
}

QString ArgAll::webURL(){
    return "http://120.77.248.40";

}
