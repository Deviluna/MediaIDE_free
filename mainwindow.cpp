#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QColor>
#include <QTextCharFormat>
#include <QFileSystemModel>
#include <QTextCodec>
#include <QTreeView>
#include <QProcess>
#include <addfiledialog.h>
#include <QTime>
#include <createprojectdialog.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);

    rootPath="C:\\Users\\wuyuehua\\htmls";
    connect(ui->actionS,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(CreateProject()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openProject()));
    loadProject(rootPath);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openProject(){
//close();
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("打开项目"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
                   QString path = fileDialog->selectedFiles()[0];
                   path=path.replace("/","\\",Qt::CaseInsensitive);
                   ui->label_2->setText(path);
                   loadProject(path);
            }

}

void MainWindow::CreateProject(){
    CreateProjectDialog *createDialog=new CreateProjectDialog(this);
    if(createDialog->exec()){
        QString projectPath=createDialog->getProjectPath();
        QString projectName=createDialog->getProjectName();

        projectPath=projectPath.replace("/","\\",Qt::CaseInsensitive);

        QDir *project=new QDir;
        bool exist = project->exists(projectPath+"\\"+projectName);
        if(exist)
            QMessageBox::warning(this,tr("创建项目"),tr("文件夹已经存在！"));
        else
        {
            bool ok = project->mkdir(projectPath+"\\"+projectName);
            if( ok ){
                ui->label_2->setText(projectName);
                loadProject(projectPath+"\\"+projectName);
                QMessageBox::warning(this,tr("创建项目"),tr("项目创建成功！"));

                    }
            }


    }
}

void MainWindow::loadProject(QString path){
    rootPath=path;
    setTreeview();
   // ui->textEdit->setEnabled(false);

}

void MainWindow::update(){
    setTreeview();

}
void MainWindow::setTreeview(){

    model= new QFileSystemModel;
    model->setRootPath(rootPath);
   // model->
    ui->treeView->setModel(model);

    ui->treeView->setRootIndex(model->index(rootPath));
    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectFile(const QModelIndex)));
}

void MainWindow::selectFile(const QModelIndex &index){
    //model->mkdir(index,"hahaha");
    //QModelIndex indexA = model->index(0, 0, QModelIndex());
  QString file= index.sibling(index.row(),0).data(0).toString();
    //QString file=index.data(0).toString();
    nowFile=rootPath+"\\"+file;
    loadFile(nowFile);
}

void MainWindow::on_pushButton_4_clicked()
{
    setAlign(Qt::AlignCenter);

}
void MainWindow::loadFile(QString path){\
    ui->textEdit->setEnabled(true);
    ui->label->setText(nowFile);
    ui->textEdit->setText("");

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString allStr="";
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        line = in.readLine();
        allStr+=line;
    }
    ui->textEdit->setText(allStr);

}



void MainWindow::setAlign(Qt::Alignment align){

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
void MainWindow::on_commitButton_clicked()
{
    outputFile(nowFile);
}

void MainWindow::getNowtext(){
    nowText=ui->textEdit->toHtml();

}
void MainWindow::setColor(const QColor &c){
    QTextCursor cursor = ui->textEdit->textCursor();

    ui->textEdit->setTextColor(c);
    // ui->textEdit->setTextColor();
   // cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
    ui->textEdit->setTextCursor( cursor ); // added
    //    ui->view1->setCurrentCharFormat( defcharfmt );
    ui->textEdit->setFocus();
}
void MainWindow::about(){
    QMessageBox::about(this,tr("About Media IDE"),tr("<h2>Media IDE v0.05</h2><p>only for evaluating</P><P>Copyright &copy; 2017 Deviluna Inc.</p>"));
}

void MainWindow::setUnderline(){

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

void MainWindow::addFile(QString fileName){




}

void MainWindow::setFont(){
    QFont serifFont("Times", 10, QFont::Bold);
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->setTextCursor( cursor );
    QTextCharFormat newFormat=ui->textEdit->currentCharFormat();
    newFormat.setUnderlineStyle( QTextCharFormat::SingleUnderline );
    newFormat.setFontUnderline( true );
    ui->textEdit->setCurrentCharFormat(newFormat);
    cursor.movePosition( QTextCursor::PreviousCharacter );
    ui->textEdit->setTextCursor( cursor );
    ui->textEdit->setFocus();

}

void MainWindow::outputFile(QString path){
    getNowtext();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
        return;
    }
    QTextStream out(&file);
    out<<nowText;
    file.close();
    setTreeview();
}




void MainWindow::previewHtml(QString path){
    QProcess* process = new QProcess();
   QString notepadPath = "explorer "+path;
    process->start(notepadPath);
}

void MainWindow::on_pushButton_clicked()
{
    //red
    setColor(Qt::red);
}

void MainWindow::on_pushButton_2_clicked()
{
    //black
    setFont();

}

void MainWindow::on_pushButton_3_clicked()
{
    setColor(Qt::black);

}

void MainWindow::on_pushButton_5_clicked()
{
    setAlign(Qt::AlignLeft);

}

void MainWindow::on_pushButton_6_clicked()
{
    outputFile(nowFile);
    ui->label_2->setText(nowFile);
    previewHtml(nowFile);
}

void MainWindow::on_pushButton_7_clicked()
{
    AddFileDialog *addDialog=new AddFileDialog(this);
    //addDialog->show();
    if(addDialog->exec()){
        nowFile=rootPath+"\\"+addDialog->getinput()+".html";
        loadFile(nowFile);
        outputFile(nowFile);
        setTreeview();
       // QMessageBox::information(NULL, tr("提示信息："), tr("我成功！"));

    }
}
