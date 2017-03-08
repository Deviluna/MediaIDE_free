#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QTextCodec>
#include <QTreeView>
#include <QProcess>
#include <addfiledialog.h>
#include <createprojectdialog.h>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <adddirdialog.h>
#include <QFontDialog>
#include <QColorDialog>
#include <QImage>
#include <testwidget.h>
#include <closedialog.h>
#include <QIcon>
#include <welcomedialog.h>
#include <rmdirdialog.h>
#include <testpage.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    setupMenu();
    loadProject(rootPath);
    setTreeview();
    initTabWidget();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenu(){
    connect(ui->actionS,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(createProject()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openProject()));
}

void MainWindow::initProgramme(){

    prootPath=QCoreApplication::applicationDirPath();
    ui->label_2->setText(prootPath);
    QFile file(prootPath+"\\"+"userinfo.ini");
    if(file.exists()){

    }
    else {
        firstUse();
    }

}

void MainWindow::closeAllTab(){
    for(int i=0;i<ui->tabWidget->count();i++){
        ui->tabWidget->removeTab(0);

    }


}

void MainWindow::CloseNowTab(){
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());

}

void MainWindow::initTabWidget(){

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    closeAllTab();


    //测试段函数
    TestPage *lsPage=new TestPage;
    ui->tabWidget->insertTab(ui->tabWidget->count()+1,lsPage,"test page");



}



void MainWindow::closeTab(int a){
    TestWidget *nowWidget=(TestWidget*)ui->tabWidget->widget(a);
    if(nowWidget->changed()){
        CloseDialog *closeDialog=new CloseDialog(this);
        closeDialog->setWindowTitle(tr("提示窗口"));
        if(closeDialog->exec()){
            //之后窗口的逻辑归窗口管了
        }
    }
    else
        ui->tabWidget->removeTab(a);
}

QWidget* MainWindow::Page(){
    return ui->tabWidget->currentWidget();
}


void MainWindow::openProject(){
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

void MainWindow::OpenProject(){
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

void MainWindow::firstUse(){
    WelcomeDialog *wDialog=new WelcomeDialog(this);
    wDialog->exec();
}

void MainWindow::createProject(){
   CreateProject();
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
            QMessageBox::warning(this,tr("创建项目"),tr("分类已经存在！"));
        else
        {
            bool ok = project->mkdir(projectPath+"\\"+projectName);
            if( ok ){
                ui->label_2->setText(projectName);
                loadProject(projectPath+"\\"+projectName);
                QString path=rootPath+"\\"+"第一篇文.html";
                QFile file(path);
                QFileInfo fi=QFileInfo(path);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
                    return;
                }
                QTextStream out(&file);
                refreshTree();
                loadFile(path);
            }
        }


    }
}

void MainWindow::loadProject(QString path){
    rootPath=path;
    nowPath=path;
    refreshTree();
    closeAllTab();
}




void MainWindow::update(){
    refreshTree();

}

void MainWindow::refreshTree(){
    model= new QFileSystemModel;
    model->setRootPath(rootPath);
    ui->treeView->setModel(model);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex(model->index(rootPath));
}


void MainWindow::setTreeview(){

    model= new QFileSystemModel;
    model->setRootPath(rootPath);
    ui->treeView->setModel(model);
    ui->treeView->setColumnHidden(0,false);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex(model->index(rootPath));
    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectFile(const QModelIndex)));
}

void MainWindow::selectFile(const QModelIndex &index){

    if(!model->isDir(index)){
        QString file= index.sibling(index.row(),0).data(0).toString();
        nowFile=rootPath+"\\"+file;
        loadFile(nowFile);
    }
    else{
        nowPath=model->filePath(index);
    }
}


void MainWindow::about(){
    QMessageBox::about(this,tr("About Media IDE"),tr("<h2>Media IDE v0.05</h2><p>only for evaluating</P><P>Copyright &copy; 2017 Deviluna Inc.</p>"));
}

void MainWindow::loadFile(QString path){
    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        if(tw->getPath().compare(path)==0){
            ui->tabWidget->setCurrentIndex(i);
            return ;
        }
    }

    QFileInfo fi=QFileInfo(path);
    TestWidget *page=new TestWidget;
    page->loadFile(path);
    page->setRootpath(rootPath);
    ui->tabWidget->insertTab(ui->tabWidget->count()+1,page,fi.baseName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

}


void MainWindow::addFile(QString fileName){
}



void MainWindow::previewHtml(QString path){
    QProcess* process = new QProcess();
    QString notepadPath = "explorer "+path;
    process->start(notepadPath);
}

void MainWindow::on_pushButton_clicked()
{
    //测试用按钮，以下是测试使用的代码

    QStringList args;
        args.append("C:/test/test.py");
        QProcess::execute(QString("Python.exe"), args);
}

void MainWindow::addArticle(){
    AddFileDialog *addDialog=new AddFileDialog(this);
    if(addDialog->exec()){
        nowFile=model->filePath(nowIndex)+"\\"+addDialog->getinput()+".html";
        loadFile(nowFile);
        //refreshTree();
    }
}

//后期研究下槽和普通函数的关系，减少冗余

void MainWindow::on_pushButton_7_clicked()
{
    AddFileDialog *addDialog=new AddFileDialog(this);
    if(addDialog->exec()){
        nowFile=rootPath+"\\"+addDialog->getinput()+".m";
        loadFile(nowFile);
        //refreshTree();
    }
}

void MainWindow::setMenuAction(){

}



void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{


    QModelIndex index = ui->treeView->indexAt(pos);
    nowIndex=index;

    if(model->isDir(index)&&model->filePath(index).length()>0){
        m_folderMenu=new QMenu(this);

        m_folderMenu->addAction("新建分类",this,SLOT(addDir()));
        m_folderMenu->addAction("添加文章",this,SLOT(addArticle()));
        m_folderMenu->addSeparator();
        m_folderMenu->addAction("删除",this,SLOT(deleteDir()));
        m_folderMenu->exec(QCursor::pos());
    }
    else if(model->filePath(index).length()>0){

        m_fileMenu=new QMenu(this);
        //这里虽然实现，但是有问题，讲道理应该用槽来传递变量，但是直接用全局变量。
        //不合规范
        m_fileMenu->addAction("删除",this,SLOT(deleteFile()));
        m_fileMenu->exec(QCursor::pos());
    }

    else{
        nowIndex=model->index(rootPath,0);
        m_folderMenu=new QMenu(this);
        m_folderMenu->addAction("新建分类",this,SLOT(addDir()));
        m_folderMenu->addAction("添加文章",this,SLOT(addArticle()));
        m_folderMenu->exec(QCursor::pos());

    }

}

void MainWindow::deleteDir(){
    if(!model->rmdir(nowIndex)){
    RmdirDialog *rmdir=new RmdirDialog(this);
    if(rmdir->exec()){
        model->remove(nowIndex);

    }

    }
}

void MainWindow::deleteFile(){
    model->remove(nowIndex);
}

void MainWindow::addDir(){

    addDirDialog *addDialog=new addDirDialog(this);

    if(addDialog->exec()){
        QString dirName=addDialog->getinput();
        model->mkdir(nowIndex,dirName);
}
}


void MainWindow::on_pushButton_8_clicked()
{
    //讲道理类名都该大写的
    addDirDialog *addDialog=new addDirDialog(this);
    if(addDialog->exec()){
        QString dirName=addDialog->getinput();
        QDir *project=new QDir;
        bool exist = project->exists(rootPath+"\\"+dirName);
        if(exist)
            QMessageBox::warning(this,tr("添加分类"),tr("分类已经存在！"));
        else
        {
            bool ok = project->mkdir(rootPath+"\\"+dirName);
            if( ok ){
            }
            else{
                QMessageBox::warning(this,tr("创建分类"),tr("失败！"));
            }

        }
        refreshTree();

    }
}

