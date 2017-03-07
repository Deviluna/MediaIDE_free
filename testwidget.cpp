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

    getNowtext();
    QFile file(path);
    QFileInfo fi=QFileInfo(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
        return;
    }
    QTextStream out(&file);
    out<<nowText;
    file.close();
    //目前保存功能移植完毕，但是由于treeview在主界面 所以没有刷新。
    if(fi.baseName()!=ui->lineEdit->text()){
        file.rename(fi.absolutePath()+"\\"+ui->lineEdit->text()+".html");
        loadFile(fi.absolutePath()+"\\"+ui->lineEdit->text()+".html");
    }

}

void TestWidget::getNowtext(){
    nowText=ui->textEdit->toHtml();
}

QString TestWidget::getPath(){
    return nowFile;
}

QString TestWidget::loadTemplate(QString path){
    QFile file(path);
    QString allStr="";

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this,tr("注意"),tr("糟糕， 模板丢失"),QMessageBox::Ok);
    }


    QTextStream in(&file);
    QString line = in.readLine();



    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }

    return allStr;
}




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
    QString line = in.readLine();
    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }
    ui->textEdit->setText(allStr);
    preText=allStr;
    //change=false;
    change=false;

}
void TestWidget::previewHtml(QString path){
    QProcess* process = new QProcess();
    QString notepadPath = "explorer "+path;
    process->start(notepadPath);
}


void TestWidget::on_pushButton_4_clicked()
{
    outputFile(nowFile);
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
    //findWord(arg1);

}
void TestWidget::setRootpath(QString path){
    rootPath=path;

}

void TestWidget::on_pushButton_5_clicked()
{
    //这里是从模板生成html的测试文件，主要就是把标题，作者等必要标签插入，还有时间采用系统时间这样，文章主要内容从tohtml截取可以用的部分来实现。
    QString title=ui->lineEdit->text();
    QString author=ui->lineEdit_3->text();
    QString content=ui->textEdit->toPlainText();
    QString templateFile=loadTemplate(rootPath+"\\"+"template.html");
    //以上将模板库和必要数据读入


    QString tohtml=ui->textEdit->toHtml();
    QRegExp rx("<body.*>(.*)</body>");
    rx.indexIn(tohtml);
    QStringList list=rx.capturedTexts();
    QString contentHtml=tohtml;

    ui->textEdit->setText(tohtml+"i have do sth");

    templateFile.replace("%(title)s",title);
    templateFile.replace("%(author)s",author);
    templateFile.replace("%(content)s",contentHtml);
    outputTemplate(templateFile);

}

void TestWidget::outputTemplate(QString html){
    QString path=rootPath+"\\"+"templatest.html";
    QFile file(rootPath+"\\"+"templatest.html");

    QFileInfo fi=QFileInfo(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out<<html;
    file.close();
    //目前保存功能移植完毕，但是由于treeview在主界面 所以没有刷新。
    if(fi.baseName()!=ui->lineEdit->text()){
        file.rename(fi.absolutePath()+"\\"+ui->lineEdit->text()+".html");
        loadFile(fi.absolutePath()+"\\"+ui->lineEdit->text()+".html");
    }

}
