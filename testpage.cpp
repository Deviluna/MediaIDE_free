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

    QString out="";
    QJsonObject json;
    json.insert("title", "官方APP上线");
    json.insert("picurl", "http://deviluna.com/appContent/pics/pic1.jpg");
    json.insert("url", "http://deviluna.com/articles/article5.html");
    QJsonObject json2;
    json2.insert("title", "打飞机");
    json2.insert("picurl", "http://deviluna.com/appContent/pics/pic2.jpg");
    json2.insert("url", "http://120.77.248.40/plane/");
    QJsonObject json3;
    json3.insert("title", "MediaIDE测试 ");
    json3.insert("picurl", "http://deviluna.com/appContent/pics/pic3.jpg");
    json3.insert("url", "http://deviluna.com/articles/article6.html");
    QJsonArray json_array;

    json_array.insert(0, json);
    json_array.insert(1, json2);
    json_array.insert(1, json3);


    QByteArray byte_array = QJsonDocument(json_array).toJson();
    ui->textEdit->setText(QString(byte_array));
    QJsonArray json_array2 = QJsonDocument::fromJson(byte_array).array();
    for(int i = 0; i < json_array2.size(); ++i) {
        QJsonObject json = json_array2.at(i).toObject();
        out+=json.value("title").toString();
        out+=json.value("author").toString();
        out+=json.value("url").toString();
    }

    //ui->textEdit->setText(out);

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
