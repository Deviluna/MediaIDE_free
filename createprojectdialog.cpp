#include "createprojectdialog.h"
#include "ui_createprojectdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateProjectDialog)
{
    ui->setupUi(this);
   ui->lineEdit_2->setText(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

CreateProjectDialog::~CreateProjectDialog()
{
    delete ui;
}
QString CreateProjectDialog::getProjectName(){
    return ui->lineEdit->text();
}
QString CreateProjectDialog::getProjectPath(){
    return ui->lineEdit_2->text();
}

void CreateProjectDialog::firstUse(){
    setWindowTitle("初次使用：新建项目");
}

void CreateProjectDialog::on_pushButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    fileDialog->setWindowTitle(tr("打开文件夹"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
                   QString path = fileDialog->selectedFiles()[0];
                   ui->lineEdit_2->setText(path);
           }

}
