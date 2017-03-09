#include "generatedialog.h"
#include "ui_generatedialog.h"
#include <QFileInfo>
#include <QJsonObject>
#include <testwidget.h>
#include <QTextStream>
#include <QJsonDocument>
#include <QString>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <argall.h>



GenerateDialog::GenerateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateDialog)
{
    tw=new TestWidget;
    ui->setupUi(this);
    //以后啊，project下main要有一个xxx.pro的描述文件，描述项目的名字及其一些基本设定，这里就读出pro之后设置，
}

GenerateDialog::~GenerateDialog()
{
    delete ui;
}
void GenerateDialog::setProjectPath(QString path){
    projectPath=path;
}

void GenerateDialog::on_pushButton_2_clicked()
{
    close();
}

void GenerateDialog::on_pushButton_clicked()
{
    //生成时间到
    genDir(projectPath,"");
    ui->label->setText("done");
    QProcess::startDetached("explorer "+ArgAll::configPath());

    //close();
}
void GenerateDialog::genHtml(QString path,QString dirPath){
    QFileInfo fi=QFileInfo(path);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QString templateStr=ArgAll::readFile(ArgAll::getTemplatePath());
    QStringList strList=ArgAll::parseMJson(path);

    QString outputString=ArgAll::replaceTemplate(templateStr,strList);
    QString wkpath=ArgAll::configPath();
    QDir wkdir(wkpath);
    if(!wkdir.exists()){
        wkdir.mkdir(wkpath);
    }

    QDir lsdir(wkpath+dirPath);
    if(!lsdir.exists()){
        lsdir.mkpath(wkpath+dirPath);

    }

    ArgAll::outputFile(wkpath+dirPath+"\\"+strList[0]+".html",outputString);
    //tw->output(wkpath+dirPath+"\\"+title+".html",outputString);

    QDir targetDir(wkpath+dirPath);
    targetDir.mkdir("stylesheets");
    //这里要注意 css最好能复用。。。
    QFile file2(":/new/prefix1/Template/stylesheets/stylesheet.css");
    file2.copy(wkpath+dirPath+"\\stylesheets\\stylesheet.css");

}

void GenerateDialog::genDir(QString path,QString dirPath){
    QDir dir(path);
    qDebug()<<path;
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            if(mfi.suffix()=="m")
                genHtml(mfi.filePath(),dirPath);

        }else
        {
            if(mfi.fileName()!="."&&mfi.fileName()!="..")
                genDir(mfi.filePath(),dirPath+"\\"+mfi.fileName());
            //ui->label->setText(mfi.filePath());
        }
    }

}

void GenerateDialog::on_pushButton_3_clicked()
{

}
