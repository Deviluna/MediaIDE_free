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
    classifylist1="";
    classifylist2="";
    classifylist3="";
    classifylist4="";
    jsonCount=0;
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

    outputJson();

    ui->label->setText("done");
    QProcess::startDetached("explorer "+ArgAll::configPath());

    //close();
}

void GenerateDialog::genIndex(){

    QStringList toReplaceIndex;
    toReplaceIndex<<indexlist;

    QString outputString=ArgAll::getReplacedIndex(toReplaceIndex,dirList);

    ArgAll::outputFile(ArgAll::configPath()+"\\index.html",outputString);




    for(int i=0;i<dirList.length();i++){

        switch (i+1){
        case 1:
            nowIndexList=&classifylist1;
            break;
        case 2:
            nowIndexList=&classifylist2;
            break;
        case 3:
            nowIndexList=&classifylist3;
            break;
        case 4:
            nowIndexList=&classifylist4;
            break;

        }
        QStringList toReplaceClassify;
        toReplaceClassify<<*nowIndexList;
        outputString=ArgAll::getReplacedIndex(toReplaceClassify,dirList);
        ArgAll::outputFile(ArgAll::configPath()+"\\"+"classify"+QString::number(i)+".html",outputString);

        //暂时先把主页屏蔽了
        if(i==0)
            ArgAll::outputFile(ArgAll::configPath()+"\\index.html",outputString);


    }



    QDir targetDir(ArgAll::configPath());
    targetDir.mkdir("stylesheets");
    QFile file2(":/new/prefix1/Template/stylesheets/stylesheet.css");
    file2.copy(ArgAll::configPath()+"\\stylesheets\\stylesheet.css");

}

bool GenerateDialog::outputJson(){




    QByteArray byte_array = QJsonDocument(json_array).toJson();

    QString jsonP=ArgAll::configPath()+"\\json\\";

    QDir wkdir(jsonP);
    if(!wkdir.exists()){
        wkdir.mkdir(jsonP);
    }


    ArgAll::outputFile(ArgAll::configPath()+"\\json\\article.json",QString(byte_array));

    return true;
}


void GenerateDialog::genHtml(QString path,QString prefix,int index,QString dirPath){
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
    QRegExp rx("<img src=\"(file:///(.*))\"");
    rx.setMinimal(true);


    int pos=rx.indexIn(htmlContent,0);
    QStringList list=rx.capturedTexts();
    int bh=1;
    int picindex=0;

    while(pos!=-1){
        QStringList list=rx.capturedTexts();
    //复制de，下次重构时候注意
        QFile lspic(list[2]);
        QFileInfo lspicInfo(list[2]);
        QString lspicName=prefix+QString::number(index)+"_"+QString::number(picindex++)+"."+lspicInfo.suffix();
        QString lspicPath=ArgAll::configPath()+"\\"+dirPath+"\\"+lspicName;
        lspic.copy(lspicPath);
        htmlContent.replace(list[1],lspicName);
        bh++;
        pos=rx.indexIn(htmlContent,pos+1);


    }





    QRegExp rx2("<body.*>(.*)</body>");
    rx2.indexIn(htmlContent);
    QStringList list2=rx2.capturedTexts();

    strList[2]=list2[0];


    //先不进行函数的分离了，设计好再说。

    //要对strList进行修改咯

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


    if(ArgAll::outputFile(wkpath+dirPath+"\\"+prefix+QString::number(index)+".html",outputString)){
        //如果生成成功，在主页添加文章列表和链接。
        QString fName=prefix+QString::number(index)+".html";
        QStringList newList;
        QString address=dirPath+"\\"+fName;
        newList<<address<<strList[0]<<strList[5]<<strList[3];
        indexlist+=ArgAll::replacelistString(ArgAll::listString(),newList);
        *nowIndexList+=ArgAll::replacelistString(ArgAll::listString(),newList);
        //如果生成成功，那json也要加上去咯
        QJsonObject json;
        json.insert("title", strList[0]);
        QString dirPath2=dirPath;

        json.insert("url", ArgAll::webURL()+"/"+dirPath2.replace("\\","")+"/"+fName);
        json.insert("date", strList[3]);
        json.insert("author",strList[1]);

        json_array.insert(jsonCount,json);
    }

    QDir targetDir(wkpath+dirPath);
    targetDir.mkdir("stylesheets");
    //这里要注意 css最好能复用。。。


    QFile file2(":/new/prefix1/Template/stylesheets/stylesheet.css");
    file2.copy(wkpath+dirPath+"\\stylesheets\\stylesheet.css");

}

void GenerateDialog::genDir(QString path,QString dirPath){

    QDir dir(path);
    int index=0;
    int dirIndex=0;
    //dir.setFilter(QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Time);
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        //建议从这里传入参数，一个参数是文章统一前缀，一个是文章编号.
        //但是前缀得跟分类有关联。就应该是分类文件夹名

        if(mfi.isFile())
        {


            if(mfi.suffix()=="m")
                genHtml(mfi.filePath(),"article",index,dirPath);
                index++;
            //根据dirList.length()来确定classifyX的内容，有一个问题就是classifX的导航栏的信息还没有完全得到，所以这个数据要暂存，之后再统一输出.。(考虑用set嘛？)

        }else
        {
            if(mfi.fileName()!="."&&mfi.fileName()!=".."){


                QDir dir2(mfi.filePath());
                dirList<<dir2.dirName();
                switch (dirList.length()){
                case 1:
                    nowIndexList=&classifylist1;
                    break;
                case 2:
                    nowIndexList=&classifylist2;
                    break;
                case 3:
                    nowIndexList=&classifylist3;
                    break;
                case 4:
                    nowIndexList=&classifylist4;
                    break;
}
                genDir(mfi.filePath(),dirPath+"\\folder"+QString::number(dirIndex++));

            }
        }
    }

}

void GenerateDialog::on_pushButton_3_clicked()
{

}
