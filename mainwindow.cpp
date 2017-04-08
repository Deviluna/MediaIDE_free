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
#include <generatedialog.h>
#include <QStandardPaths>
#include <argall.h>
#include <qstringlist.h>
#include <renamedialog.h>
#include <QCloseEvent>
#include <dirprodialog.h>
#include <qdesktopservices.h>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


MainWindow::
MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowState(Qt::WindowMaximized);
    ui->setupUi(this);
    setupMenu();
    setTreeview();
    initTabWidget();

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadJson(){
    mstJsonObject=ArgAll::getJsonObject(mstPath);

}

void MainWindow::setupMenu(){
    //菜单连接槽函数
    connect(ui->actionS,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(createProject()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openProject()));
    connect(ui->actionX,SIGNAL(triggered()),this,SLOT(generate()));
}


void MainWindow::generate(){
    //打开构建界面
    GenerateDialog *gDialog=new GenerateDialog(this);
    gDialog->setProjectPath(rootPath);
    gDialog->setDirList();
    gDialog->exec();
}


void MainWindow::initProgramme(){
    //每次启动时候使用（被main.cpp调用）
    QFile file(ArgAll::getSettingPath());
    QStringList settings=ArgAll::parsePSTJson(ArgAll::getSettingPath());
    if(settings[0]!=""){
        loadProject(settings[0]);
    }
    else {
        firstUse();
    }
}

void MainWindow::closeAllTab(){
    int max=ui->tabWidget->count();
    for(int i=0;i<max;i++){
        ui->tabWidget->removeTab(0);
    }
}

void MainWindow::CloseNowTab(){
    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::initTabWidget(){
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    closeAllTab();
}

bool MainWindow::renameNowTab(QString newName){
    int nowIndex=ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(nowIndex,newName);
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
    {
        ui->tabWidget->removeTab(a);
    }
}

QWidget* MainWindow::Page(){
    return ui->tabWidget->currentWidget();
}


void MainWindow::setDirPro(QString path){
    //目前废弃
    DirProDialog *dpd=new DirProDialog(this);
    dpd->setPath(path);
    dpd->exec();
}

void MainWindow::openProject(){
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    fileDialog->setWindowTitle(tr("打开项目"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        ui->label_2->setText(path);
        loadProject(path);
    }

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    //关闭时候，存储已经打开的标签页。下次打开软件时候，这些标签页会自动打开
    QJsonArray openedTabs;
    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        openedTabs.push_back(tw->getPath());
    }

    //把openedTab改成JsonArray形式



    mstJsonObject.remove("openedTabs");
    mstJsonObject.insert("openedTabs",openedTabs);

    mstJsonObject.remove("lastIndex");
    mstJsonObject.insert("lastIndex",ui->tabWidget->currentIndex());



    ArgAll::outputJson(mstPath,mstJsonObject);

   // ArgAll::modifyJson(mstPath,"openedTab",openedTabs);


    // 这个时候再存mst啦。





}



void MainWindow::OpenProject(){
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("打开项目"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
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
        QDir *project=new QDir;
        bool exist = project->exists(projectPath+"/"+projectName);
        if(exist)
            QMessageBox::warning(this,tr("创建项目"),tr("分类已经存在！"));
        else
        {
            bool ok = project->mkdir(projectPath+"/"+projectName);
            if( ok ){
                loadProject(projectPath+"/"+projectName);
                project->mkdir(projectPath+"/"+projectName+"/"+"分类一");
                QString path=projectPath+"/"+projectName+"/分类一/第一篇文章.m";
                ArgAll::createFile(path);
                loadFile(path);
                //新建需要创建用户适配的json，之后projectName存在json里面，可以修改，每次load时候又显示出来。
                //这里以后要修改projectpath的地方
                mstPath=projectPath+"/"+projectName+"/"+ArgAll::settingName();
                ArgAll::createFile(mstPath);
                ArgAll::modifyJson(mstPath,"name",projectName);
            }
        }
    }
}

void MainWindow::loadProject(QString path){
    rootPath=path;
    nowPath=path;
    mstPath=path+"/"+ArgAll::settingName();
    refreshTree();
    closeAllTab();
    //为了保证鲁棒性，这里必须做修改。
    mstList=ArgAll::parseMSTJson(mstPath);
    ui->label_2->setText(mstList[0]);
    //加一段，写入path到json中。
    ArgAll::modifyPSTJson("lastProject",path);
    loadJson();
    QJsonArray openedTabs=mstJsonObject.value("openedTabs").toArray();
    for (int i=0;i<openedTabs.size(); i++){
        QString tabPath=openedTabs.at(i).toString();
        if(tabPath.length()>0)
            loadFile(tabPath);
    }
    ui->tabWidget->setCurrentIndex(mstJsonObject.value("lastIndex").toInt());
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
        loadFile(model->filePath(index));
    }
    else{
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


    //这个m要从一个统一的位置获取
    if(fi.suffix()=="m"){

        TestWidget *page=new TestWidget;
        page->loadFile(path);
        page->setRootpath(rootPath);
        ui->tabWidget->insertTab(ui->tabWidget->count()+1,page,fi.baseName());
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }

    if(fi.fileName()==ArgAll::settingName()){
        //点击打开项目设置窗口
        //
        qDebug()<<"点击打开项目设置窗口";


    }
}

void MainWindow::addArticle(){
    AddFileDialog *addDialog=new AddFileDialog(this);
    if(addDialog->exec()){
        loadFile(model->filePath(nowIndex)+"/"+addDialog->getinput()+".m");
    }
}


void MainWindow::openInExplorer(){
    qDebug()<<model->filePath(nowIndex);
    QDesktopServices::openUrl(QUrl::fromLocalFile(model->filePath(nowIndex)));
}

void MainWindow::renameDir(){
    RenameDialog *rd=new RenameDialog(this);
    rd->setNowName(model->fileName(nowIndex));
    rd->exec();
}


void MainWindow::renameNowDir(QString newName){
    //重命名
    QFileInfo qfi(model->filePath(nowIndex));
    QString path=model->filePath(nowIndex);
    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        QFileInfo lsqfi(tw->getPath());
        if(lsqfi.canonicalPath()==qfi.filePath()){
            ui->tabWidget->removeTab(i);
        }
    }
    QFile::rename(qfi.filePath(),qfi.canonicalPath()+"/"+newName);
}


void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{


    QModelIndex index = ui->treeView->indexAt(pos);
    nowIndex=index;

    if(model->isDir(index)&&model->filePath(index).length()>0){
        m_folderMenu=new QMenu(this);
        //m_folderMenu->addAction("新建分类",this,SLOT(addDir()));
        m_folderMenu->addAction("添加文章",this,SLOT(addArticle()));
        m_folderMenu->addSeparator();
        m_folderMenu->addAction("在本地打开",this,SLOT(openInExplorer()));
        m_folderMenu->addAction("重命名",this,SLOT(renameDir()));
        m_folderMenu->addAction("删除",this,SLOT(deleteDir()));
        m_folderMenu->exec(QCursor::pos());
    }
    else if(model->filePath(index).length()>0){
        QFileInfo qfi(model->filePath(index));
        if(qfi.suffix()!="m")
            return ;
        m_fileMenu=new QMenu(this);
        m_fileMenu->addAction("删除",this,SLOT(deleteFile()));
        m_fileMenu->exec(QCursor::pos());
    }

    else{
        nowIndex=model->index(rootPath,0);
        m_folderMenu=new QMenu(this);
        m_folderMenu->addAction("新建分类",this,SLOT(addDir()));
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
    QString path=model->filePath(nowIndex);
    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        if(tw->getPath().compare(path)==0){
            ui->tabWidget->removeTab(i);
        }
    }
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
    addDirDialog *addDialog=new addDirDialog(this);
    if(addDialog->exec()){
        QString dirName=addDialog->getinput();
        QDir *project=new QDir;

        QString dirPath=rootPath+"/"+dirName;
        bool exist = project->exists(dirPath);
        if(exist)
            QMessageBox::warning(this,tr("添加分类"),tr("分类已经存在！"));
        else
        {
            bool ok = project->mkdir(dirPath);
            if( ok ){

            }
            else{
                QMessageBox::warning(this,tr("创建分类"),tr("失败！"));
            }
        }

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    closeAllTab();
}


