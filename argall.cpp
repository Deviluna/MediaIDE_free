#include "argall.h"
#include <QDesktopServices>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>

ArgAll::ArgAll()
{
    // 全局调用的参数都在这里设置，但是目前开发先不重构了



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
        return "bad read!!!";
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
