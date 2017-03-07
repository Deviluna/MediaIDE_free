#include "testpage.h"
#include "ui_testpage.h"
#include <QFile>
#include <QMessageBox>
#include <QIODevice>
#include <QTextStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>


TestPage::TestPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestPage)
{
    ui->setupUi(this);
}

TestPage::~TestPage()
{
    delete ui;
}


void TestPage::test(){


    QJsonObject json;
    json.insert("name", QString("Qt"));
    json.insert("version", 5);
    json.insert("windows", true);

    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);


    QString data=loadJson("C:\\test\\testjson.html");
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(data.toUtf8(), &json_error);
    QString out="";
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            QJsonObject obj = parse_doucment.object();
            if(obj.contains("name"))
            {
                QJsonValue name_value = obj.take("name");
                if(name_value.isString())
                {
                    QString name = name_value.toString();
                    out+=name;
                }
            }
            if(obj.contains("version"))
            {
                QJsonValue version_value = obj.take("version");
                if(version_value.isDouble())
                {
                    int version = version_value.toVariant().toInt();
                    out+=QString::number(version);
                }
            }
            if(obj.contains("windows"))
            {
                QJsonValue version_value = obj.take("windows");
                if(version_value.isBool())
                {
                    bool flag = version_value.toBool();

                }
            }
        }
    }
    else out="error";
ui->textEdit->setText(out);

}

QString TestPage::loadJson(QString path){
    QFile file(path);
    QString allStr="";

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this,tr("注意"),tr("糟糕， 模板丢失"),QMessageBox::Ok);
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();
    allStr+=line;
    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }
    ui->textEdit_2->setText(allStr);
    return allStr;
}

void TestPage::on_pushButton_clicked()
{
    test();
}
