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
#include <QRegExp>



GenerateDialog::GenerateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateDialog)
{
    tw=new TestWidget;
    ui->setupUi(this);
    indexlist="";
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
    genIndex();


    ui->label->setText("done");
    QProcess::startDetached("explorer "+ArgAll::configPath());

    //close();
}

void GenerateDialog::genIndex(){
    QStringList toReplaceIndex;
    toReplaceIndex<<indexlist;
    QString outputString=ArgAll::getReplacedIndex(toReplaceIndex);
    ArgAll::outputFile(ArgAll::configPath()+"\\index.html",outputString);



    QDir targetDir(ArgAll::configPath());
    targetDir.mkdir("stylesheets");
    QFile file2(":/new/prefix1/Template/stylesheets/stylesheet.css");
    file2.copy(ArgAll::configPath()+"\\stylesheets\\stylesheet.css");

}


void GenerateDialog::genHtml(QString path,QString dirPath){
    QFileInfo fi=QFileInfo(path);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QString templateStr=ArgAll::readFile(ArgAll::getTemplatePath());
    QStringList strList=ArgAll::parseMJson(path);



    //这里需要有对content中图片src的修复，并且要把图片复制到同目录下，最好还要考虑图片名字重构。
    //既然复制粘贴的图有效，实在不行在，读取图片的层中间强行加入复制粘贴就行了。。。

    QString htmlContent=strList[2];
    QRegExp rx("<img src=\"(.+)?\"");
    rx.indexIn(htmlContent);
    rx.setMinimal(true);
    QStringList list=rx.capturedTexts();
    //qDebug()<<htmlContent;
    for(int i=0;i<list.size();i++)
    qDebug()<<list[i]<<"测试图片用的"<<strList[0];


    //先不进行函数的分离了，设计好再说。

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

    if(ArgAll::outputFile(wkpath+dirPath+"\\"+strList[0]+".html",outputString)){
        //如果生成成功，在主页添加文章列表和链接。
        QStringList newList;
        QString address=dirPath+"\\"+strList[0]+".html";
        newList<<address<<strList[0]<<strList[5]<<strList[3];
        indexlist+=ArgAll::replacelistString(ArgAll::listString(),newList);
    }
                  QDir targetDir(wkpath+dirPath);
        targetDir.mkdir("stylesheets");
        //这里要注意 css最好能复用。。。
        QFile file2(":/new/prefix1/Template/stylesheets/stylesheet.css");
        file2.copy(wkpath+dirPath+"\\stylesheets\\stylesheet.css");

    }

    void GenerateDialog::genDir(QString path,QString dirPath){
        QDir dir(path);
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
