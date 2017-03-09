#ifndef ARGALL_H
#define ARGALL_H

#include <QString>

class ArgAll
{
public:
    ArgAll();
    QString documentPath();
    QString configPath();
    static QString  readFile(QString path);
    static bool  outputFile(QString path,QString content);
    static QString getTemplatePath();
};

#endif // ARGALL_H
