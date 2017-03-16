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
#include <generatedialog.h>
#include <QStandardPaths>
#include <argall.h>
#include <qstringlist.h>
#include <renamedialog.h>
#include <QCloseEvent>
#include <dirprodialog.h>


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

void MainWindow::setupMenu(){
    connect(ui->actionS,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionCreate,SIGNAL(triggered()),this,SLOT(ject()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openProject()));
    connect(ui->actionX,SIGNAL(triggered()),this,SLOT(generate()));
}


void MainWindow::generate(){
    GenerateDialog *gDialog=new GenerateDialog(this);
    gDialog->setProjectPath(rootPath);
    gDialog->exec();
}


void MainWindow::initProgramme(){
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
        //closeTab(i);
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
        path=path.replace("/","\\",Qt::CaseInsensitive);
        ui->label_2->setText(path);
        loadProject(path);
    }

}


void MainWindow::closeEvent(QCloseEvent *event)
{

    QString lastTabs="";


    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        if(i!=0)lastTabs+=",";
        lastTabs+=tw->getPath();
    }

    ArgAll::modifyJson(mstPath,"openedTab",lastTabs);


}



void MainWindow::OpenProject(){
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

    //这里要创建一个json，好好读写，应该再argall里面设置一个对json读写的方法，得到输入输出。
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
                loadProject(projectPath+"\\"+projectName);
                project->mkdir(projectPath+"\\"+projectName+"\\"+"分类一");
                QString path=projectPath+"\\"+projectName+"\\分类一\\第一篇文章.m";
                ArgAll::createFile(path);
                loadFile(path);
                //新建需要创建用户适配的json，之后projectName存在json里面，可以修改，每次load时候又显示出来。
                //这里以后要修改projectpath的地方
                mstPath=projectPath+"\\"+projectName+"\\"+ArgAll::settingName();
                ArgAll::createFile(mstPath);
                ArgAll::modifyJson(mstPath,"name",projectName);
            }
        }
    }
}

void MainWindow::loadProject(QString path){
    rootPath=path;
    nowPath=path;
    mstPath=path+"\\"+ArgAll::settingName();
    refreshTree();
    closeAllTab();
    //为了保证鲁棒性，这里必须做修改。
    mstList=ArgAll::parseMSTJson(mstPath);
    ui->label_2->setText(mstList[0]);
    //加一段，写入path到json中。


    ArgAll::modifyPSTJson("lastProject",path);
    QStringList openedTabs=ArgAll::parseMSTJson(mstPath)[1].split(",");

    for (int i=0;i<openedTabs.length(); i++){
        QString tabPath=openedTabs[i];
        if(tabPath.length()>0)
            loadFile(tabPath);
    }

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
        loadFile(model->filePath(index).replace("/","\\"));
    }
    else{
        // nowPath=model->filePath(index);
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

    if(fi.fileName()==ArgAll::dirProName()){
        setDirPro(path);
    }




    if(fi.suffix()!="m")
        return ;

    TestWidget *page=new TestWidget;
    page->loadFile(path);
    page->setRootpath(rootPath);
    ui->tabWidget->insertTab(ui->tabWidget->count()+1,page,fi.baseName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

}


void MainWindow::addFile(QString fileName){
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
        loadFile(model->filePath(nowIndex).replace("/","\\")+"\\"+addDialog->getinput()+".m");
    }
}

//后期研究下槽和普通函数的关系，减少冗余

void MainWindow::on_pushButton_7_clicked()
{

}

void MainWindow::setMenuAction(){

}


void MainWindow::openInExplorer(){
    QProcess::startDetached("explorer "+model->filePath(nowIndex).replace("/","\\"));

}

void MainWindow::renameDir(){
    RenameDialog *rd=new RenameDialog(this);
    rd->setNowName(model->fileName(nowIndex));
    rd->exec();


}


void MainWindow::renameNowDir(QString newName){
    //重命名
    QFileInfo qfi(model->filePath(nowIndex));



    QString path=model->filePath(nowIndex).replace("/","\\");

    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        QFileInfo lsqfi(tw->getPath());
        if(lsqfi.canonicalPath()==qfi.filePath()){
            ui->tabWidget->removeTab(i);

        }
    }





    QFile::rename(qfi.filePath(),qfi.canonicalPath()+"\\"+newName);




    //对应标签页面需要修改和mst文件
    //应该只需要修改标签页，就自动适配好reload


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
        //这里虽然实现，但是有问题，讲道理应该用槽来传递变量，但是直接用全局变量。
        //不合规范
        //m_fileMenu->addAction("在本地打开",this,SLOT(openInExplorer()));


        //重命名，load当前，之后focus移动到名字属性那里。


        m_fileMenu->addAction("删除",this,SLOT(deleteFile()));
        m_fileMenu->exec(QCursor::pos());
    }

    else{
        nowIndex=model->index(rootPath,0);
        m_folderMenu=new QMenu(this);
        m_folderMenu->addAction("新建分类",this,SLOT(addDir()));
        //  m_folderMenu->addAction("添加文章",this,SLOT(addArticle()));
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

    QString path=model->filePath(nowIndex).replace("/","\\");
    for(int i=0;i<ui->tabWidget->count();i++){
        TestWidget *tw=(TestWidget*)ui->tabWidget->widget(i);
        if(tw->getPath().compare(path)==0){
            ui->tabWidget->removeTab(i);
        }


    }
    model->remove(nowIndex);
    //如果开着的话，要在tab里面关闭

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


    //首先要判断分类是否达到最大,如果超过规定，就不可创建了。




    //讲道理类名都该大写的
    addDirDialog *addDialog=new addDirDialog(this);
    if(addDialog->exec()){
        QString dirName=addDialog->getinput();
        QDir *project=new QDir;

        QString dirPath=rootPath+"\\"+dirName;
        bool exist = project->exists(dirPath);
        if(exist)
            QMessageBox::warning(this,tr("添加分类"),tr("分类已经存在！"));
        else
        {
            bool ok = project->mkdir(dirPath);
            if( ok ){
                //ok以后要在当前分类下创建分类属性的文件
                QString dirProPath=dirPath+"\\"+ArgAll::dirProName();
                ArgAll::createFile(dirProPath);

                ArgAll::modifyJson(dirProPath,"order","10");



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

void MainWindow::on_tabWidget_customContextMenuRequested(const QPoint &pos)
{

}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{

}
