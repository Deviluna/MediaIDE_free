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
#include <QStandardItemModel>
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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    connect(ui->actionS,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(createProject()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openProject()));
    loadProject(rootPath);
    setTreeview();
    initTabWidget();
}
MainWindow::~MainWindow()
{
    delete ui;
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
    //等下分开独立成舒适化函数
    for(int i=0;i<=ui->tabWidget->count();i++){
        ui->tabWidget->removeTab(i);
    }
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    closeAllTab();

}



void MainWindow::closeTab(int a){
    //TestWidget *nowWidget=(TestWidget)ui->tabWidget->(a);
    TestWidget *nowWidget=(TestWidget*)ui->tabWidget->widget(a);

    if(nowWidget->changed()){
        //这一段希望是添加改动以后的提醒保存按钮，但是有两处技术问题，不知道怎么交互

        CloseDialog *closeDialog=new CloseDialog(this);
        closeDialog->setWindowTitle(tr("提示窗口"));
        if(closeDialog->exec()){



        }




    }
    else
        ui->tabWidget->removeTab(a);
}

QWidget* MainWindow::Page(){

    return ui->tabWidget->currentWidget();
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
    wDialog->show();
    //connect(wDialog->ui->push,)


    /*
     * CreateProjectDialog *createDialog=new CreateProjectDialog(this);
    createDialog->firstUse();
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
                // QMessageBox::warning(this,tr("创建项目"),tr("项目创建成功！"));

            }
        }


    }
*/
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
            QMessageBox::warning(this,tr("创建项目"),tr("文件夹已经存在！"));
        else
        {
            bool ok = project->mkdir(projectPath+"\\"+projectName);
            if( ok ){
                ui->label_2->setText(projectName);
                loadProject(projectPath+"\\"+projectName);
              //  QMessageBox::warning(this,tr("创建项目"),tr("项目创建成功！"));

                QString path=rootPath+"\\"+"第一篇文.html";
                //初始化文件
                QFile file(path);
                QFileInfo fi=QFileInfo(path);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QMessageBox::information(NULL, tr("提示信息："), tr("文件打开失败！"));
                    return;
                }
                QTextStream out(&file);
                QString fir="下一个要解决的问题，重复的page（应该easy的，顶多遍历），接着还要解决存储问题";
                out<<fir;
                //out<<"下一个要解决的问题，重复的page（应该easy的，顶多遍历），接着还要解决存储问题";
                file.close();
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
    // ui->textEdit->setEnabled(false);

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
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex(model->index(rootPath));
    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectFile(const QModelIndex)));
}

void MainWindow::selectFile(const QModelIndex &index){
    //model->mkdir(index,"hahaha");
    //QModelIndex indexA = model->index(0, 0, QModelIndex());

    if(!model->isDir(index)){
        QString file= index.sibling(index.row(),0).data(0).toString();
        //QString file=index.data(0).toString();
        nowFile=rootPath+"\\"+file;
        loadFile(nowFile);
    }
    else{
        nowPath=model->filePath(index);
        //ui->label->setText(nowPath);
        // cleanText();
    }
}


void MainWindow::about(){
    QMessageBox::about(this,tr("About Media IDE"),tr("<h2>Media IDE v0.05</h2><p>only for evaluating</P><P>Copyright &copy; 2017 Deviluna Inc.</p>"));
}

void MainWindow::loadFile(QString path){

    QFileInfo fi=QFileInfo(path);
    TestWidget *page=new TestWidget;
    page->loadFile(path);
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

}


void MainWindow::on_pushButton_7_clicked()
{
    AddFileDialog *addDialog=new AddFileDialog(this);
    if(addDialog->exec()){
        nowFile=rootPath+"\\"+addDialog->getinput()+".html";

        loadFile(nowFile);

        //  outputFile(nowFile);

        refreshTree();

    }
}

void MainWindow::setMenuAction(){
    m_folderMenu=new QMenu(this);
    m_fileMenu=new QMenu(this);
    QAction *ac = nullptr;
    ac = new QAction(QStringLiteral("重命名"),this);
    m_folderMenu->addAction(ac);

    ac = new QAction(QStringLiteral("删除"),this);
    m_folderMenu->addAction(ac);


    ac = new QAction(QStringLiteral("编辑"),this);
    m_fileMenu->addAction(ac);

    ac = new QAction(QStringLiteral("重命名"),this);
    m_fileMenu->addAction(ac);

    //ac = new QAction(QStringLiteral("删除"),this,SLOT(close()));

    m_fileMenu->addAction(ac);

}




void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{


    QModelIndex index = ui->treeView->indexAt(pos);
    ui->label_2->setText(model->filePath(index));
    nowIndex=index;


    if(model->isDir(index)){
        m_folderMenu=new QMenu(this);
        m_folderMenu->addAction("删除",this,SLOT(deleteDir()));
        m_folderMenu->exec(QCursor::pos());




    }
    else{

        m_fileMenu=new QMenu(this);
        //这里虽然实现，但是有问题，讲道理应该用槽来传递变量，但是直接用全局变量。
        //不合规范

        m_fileMenu->addAction("删除",this,SLOT(deleteFile()));

        m_fileMenu->exec(QCursor::pos());
    }
}

void MainWindow::deleteDir(){
    model->rmdir(nowIndex);
    refreshTree();
}

void MainWindow::deleteFile(){
    model->remove(nowIndex);
    refreshTree();
}

void MainWindow::on_pushButton_8_clicked()
{
    //讲道理类名都该大写的

    addDirDialog *addDialog=new addDirDialog(this);

    if(addDialog->exec()){
        QString dirName=addDialog->getinput();
        QDir *project=new QDir;
        bool exist = project->exists(nowPath+"\\"+dirName);
        if(exist)
            QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
        else
        {
            bool ok = project->mkdir(nowPath+"\\"+dirName);
            if( ok ){
                //ui->label_2->setText(projectName);
                //loadProject(projectPath+"\\"+projectName);
                //QMessageBox::warning(this,tr("创建项目"),tr("项目创建成功！"));

            }
            else{
                QMessageBox::warning(this,tr("创建文件夹"),tr("失败！"));
            }

        }
        refreshTree();

    }
}

