#include "testwidget.h"
#include "ui_testwidget.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <mainwindow.h>
#include <QProcess>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QTextCursor>
#include <QRegExp>
#include <QJsonObject>
#include <QJsonDocument>
#include <argall.h>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::on_pushButton_clicked()
{
    save();
}

void TestWidget::save(){
    outputFile(nowFile);

}

void TestWidget::findWord(QString word){



    if(ui->textEdit->find(word,QTextDocument::FindCaseSensitively))
    {
        QPalette palette = ui->textEdit->palette();
        palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
        ui->textEdit->setPalette(palette);
        //ui->textEdit->setCursor();
    }

    else
    {
        QTextCursor cur=ui->textEdit->textCursor();
        cur.setPosition(2,QTextCursor::MoveAnchor);
        ui->textEdit->setTextCursor(cur);
        if(ui->textEdit->find(word,QTextDocument::FindCaseSensitively))
        {
            QPalette palette = ui->textEdit->palette();
            palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
            ui->textEdit->setPalette(palette);
            //ui->textEdit->setCursor();
        }


        else       QMessageBox::information(this,tr("注意"),tr("没有找到内容：")+word,QMessageBox::Ok);
    }

}


void TestWidget::outputFile(QString path){
    //建议都改掉，都传MAP或者stringlist，现在参数太多了。
    //如果用stringlist性能应该会好一点，后面跟着改东西应该也少一点，用stringlist把。

    QString content=ui->textEdit->toHtml();
    QString author=ui->lineEdit_3->text();
    QString title=ui->lineEdit->text();
    QString date="日期功能待加入";
    QJsonObject json;
    json.insert("title", title);
    json.insert("author", author);
    json.insert("date", date);
    json.insert("content", content);
    QString nowText=QString(QJsonDocument(json).toJson());

    QFile file(path);
    QFileInfo fi=QFileInfo(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<nowText;
    file.close();
    //目前保存功能移植完毕，但是由于treeview在主界面 所以没有刷新。
    if(fi.baseName()!=ui->lineEdit->text()){
        file.rename(fi.absolutePath()+"\\"+ui->lineEdit->text()+".m");
        loadFile(fi.absolutePath()+"\\"+ui->lineEdit->text()+".m");
    }

}

void TestWidget::getNowtext(){
    nowText=ui->textEdit->toHtml();
}

QString TestWidget::getPath(){
    return nowFile;
}



//此处打算大改，修改成对json形式的存储。
//行有余力，就对数据格式加密
void TestWidget::loadFile(QString path){
    QFileInfo fi=QFileInfo(path);
    nowFile=path;
    ui->lineEdit->setText(fi.baseName());
    ui->textEdit->setText("");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        outputFile(path);
        return;
    }
    QString allStr="";
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line = in.readLine();
    allStr+=line;
    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }
    //以上读完了all string


    QJsonObject json=QJsonDocument::fromJson(allStr.toUtf8()).object();
    ui->lineEdit->setText(json.value("title").toString());
    ui->lineEdit_3->setText(json.value("author").toString());
    ui->textEdit->setText(json.value("content").toString());

    change=false;
}
void TestWidget::previewHtml(QString path){



    QString templateStr=ArgAll::readFile(":/new/prefix1/Template/template.html");

    QString title=ui->lineEdit->text();
    QString author=ui->lineEdit_3->text();
    QString date="测试数据";
    QString tohtml=ui->textEdit->toHtml();
    QRegExp rx("<body.*>(.*)</body>");
    rx.indexIn(tohtml);
    QStringList list=rx.capturedTexts();
    QString contentHtml=tohtml;
    //得到四个填充元素

    // QFile file(":/new/prefix1/Template/template.html");
    QStringList strList;
    strList<<title<<author<<date<<tohtml;
    QString outputString=ArgAll::replaceTemplate(templateStr,strList);
    QString wkpath=ArgAll::configPath();
    QDir wkdir(wkpath);
    if(!wkdir.exists()){
        wkdir.mkdir(wkpath);
    }

    ArgAll::outputFile(wkpath+"\\temp.html",outputString);
    //output(wkpath+"\\temp.html",outputString);
    QDir targetDir(wkpath);
    targetDir.mkdir("stylesheets");

    QFile file(":/new/prefix1/Template/stylesheets/stylesheet.css");
    file.copy(wkpath+"\\stylesheets\\stylesheet.css");

    QProcess* process = new QProcess();
    QString notepadPath = "explorer  "+wkpath+"\\temp.html";
    process->start(notepadPath);

}


void TestWidget::output(QString path, QString str){

    QFile file(path);
    QFileInfo fi=QFileInfo(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<str;
    file.close();
}





void TestWidget::on_pushButton_4_clicked()
{
    //outputFile(nowFile);
    // ui->label->setText(nowFile);
    previewHtml(nowFile);
}


void TestWidget::setAlign(Qt::Alignment align){

    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->setTextCursor( cursor );
    QTextBlockFormat tbFmt=cursor.blockFormat();
    tbFmt.setAlignment(Qt::AlignCenter);
    tbFmt.setAlignment(align);
    cursor.setBlockFormat(tbFmt);
    ui->textEdit->setTextCursor( cursor ); // added
    //    ui->view1->setCurrentCharFormat( defcharfmt );
    ui->textEdit->setFocus();

}

void TestWidget::on_toolButton_clicked()
{
    setAlign(Qt::AlignCenter);
}

void TestWidget::on_toolButton_3_clicked()
{
    setAlign(Qt::AlignLeft);

}

void TestWidget::on_toolButton_4_clicked()
{
    setAlign(Qt::AlignRight);

}
void TestWidget::setBold(){
    QFont preFont=ui->textEdit->currentFont();
    preFont.setBold(preFont.bold()?false:true);
    setFont(preFont);
}


void TestWidget::setFont(const QFont &font){
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->setTextCursor( cursor );  // added
    ui->textEdit->setCurrentFont(font);
    ui->textEdit->setTextCursor( cursor );
    ui->textEdit->setFocus();
}


void TestWidget::on_toolButton_2_clicked()
{
    setBold();
}

void TestWidget::on_toolButton_5_clicked()
{
    setUnderline();
}

void TestWidget::setUnderline(){

    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->setTextCursor( cursor );  // added
    QTextCharFormat newFormat=ui->textEdit->currentCharFormat();
    newFormat.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    newFormat.setFontUnderline( true );
    //  newFormat.AlignMiddle=true;
    ui->textEdit->setCurrentCharFormat(newFormat);
    // cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
    ui->textEdit->setTextCursor( cursor ); // added
    //    ui->view1->setCurrentCharFormat( defcharfmt );
    ui->textEdit->setFocus();
}

void TestWidget::setColor(const QColor &c){
    QTextCursor cursor = ui->textEdit->textCursor();

    ui->textEdit->setTextColor(c);
    // ui->textEdit->setTextColor();
    // cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
    ui->textEdit->setTextCursor( cursor ); // added
    //    ui->view1->setCurrentCharFormat( defcharfmt );
    ui->textEdit->setFocus();
}


void TestWidget::setBigger(){
    QFont preFont=ui->textEdit->currentFont();
    preFont.setPointSize((preFont.pointSize()+5));
    setFont(preFont);
}

void TestWidget::setSmaller(){
    QFont preFont=ui->textEdit->currentFont();
    if(preFont.pointSize()>5)
        preFont.setPointSize((preFont.pointSize()-5));
    setFont(preFont);
}

void TestWidget::on_toolButton_6_clicked()
{
    setBigger();
}

void TestWidget::on_toolButton_7_clicked()
{
    setSmaller();
}

void TestWidget::on_toolButton_8_clicked()
{
    setColor(Qt::red);

}

void TestWidget::on_pushButton_3_clicked()
{
    QColor color = QColorDialog::getColor(ui->textEdit->textColor(),this);  //初始颜色为绿色
    setColor(color);
}

void TestWidget::on_pushButton_2_clicked()
{
    QFont preFont=ui->textEdit->currentFont();

    bool ifok;

    QFont font = QFontDialog::getFont(&ifok,preFont,this,"选择字体",QFontDialog::ProportionalFonts);

    setFont(font);
}

void TestWidget::on_toolButton_9_clicked()
{
    setFont(quickFont);

}

void TestWidget::on_toolButton_11_clicked()
{
    setColor(quickColor);
}

void TestWidget::on_toolButton_10_clicked()
{
    setQuickFont();
}

void TestWidget::setQuickFont(){
    bool ifok;
    QFont font = QFontDialog::getFont(&ifok,quickFont,this,"选择字体",QFontDialog::ProportionalFonts);
    quickFont=font;
}



void TestWidget::on_toolButton_12_clicked()
{
    setQuickColor();
}

void TestWidget::setQuickColor(){
    QColor color = QColorDialog::getColor(quickColor,this);  //初始颜色为绿色
    quickColor=color;
}


void TestWidget::insertImage(){

    QString path;
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("插入图片"));
    fileDialog->setFileMode(QFileDialog::ExistingFile);

    QStringList filters;
    filters<<"*.jpg *.png *.bmp";
    fileDialog->setNameFilters(filters);
    fileDialog->setFilter(QDir::Dirs|QDir::Files);
    if(fileDialog->exec() == QDialog::Accepted) {
        path = fileDialog->selectedFiles()[0];
    }
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->setTextCursor( cursor );  // added
    QImage image;
    image.load(path);
    // image=image.scaled(100,100,Qt::IgnoreAspectRatio,Qt::FastTransformation);
    cursor.insertImage(image,"name");
    ui->textEdit->setTextCursor( cursor );
    ui->textEdit->setFocus();

}

void TestWidget::on_toolButton_13_clicked()
{
    insertImage();
}

bool TestWidget::changed(){
    return change;
}

void TestWidget::on_textEdit_textChanged()
{
    //目前只是粗略的判断改动，以后对于是否进行更改的判断要更细致（比较前后两个文件是否相同）

    change=true;

}

void TestWidget::on_toolButton_14_clicked()
{
    findWord(ui->lineEdit_2->text());
}

void TestWidget::on_lineEdit_2_textChanged(const QString &arg1)
{

}
void TestWidget::setRootpath(QString path){
    rootPath=path;

}

void TestWidget::on_pushButton_5_clicked()
{


}


