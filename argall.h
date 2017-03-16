#ifndef ARGALL_H
#define ARGALL_H

#include <QString>
#include <QWidget>


class ArgAll
{
public:
    ArgAll();
    static QString documentPath();
    static QString configPath();
    static QString  readFile(QString path);
    static bool  outputFile(QString path,QString content);
    static QString getTemplatePath();
    static QString replaceTemplate(QString temp, QStringList strList);
    static QStringList parseMJson(QString mfilePath);
    static QStringList parsePSTJson(QString pstfilePath);
    static QString getSettingPath();
    static bool modifyPSTJson(QString key,QString value);
    static bool modifyJson(QString Path,QString key,QString value);
    static bool createFile(QString path);
    static QStringList parseMSTJson(QString mstfilePath);
    static bool addMSTTab(QString MSTPath,QString path);
    static bool removeMSTTab(QString MSTPath,int i);//可以考虑使用ini i
    static QString settingName();
    static QString listString();
    static QString replacelistString(QString temp,QStringList strList);
    static QString getReplacedIndex(QStringList strList,QStringList dirList);
    static QString webURL();
    static QString insertImage(QWidget *parent);
};

#endif // ARGALL_H
