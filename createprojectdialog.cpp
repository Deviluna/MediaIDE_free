#include "createprojectdialog.h"
#include "ui_createprojectdialog.h"
#include <QFileDialog>
#include <QMessageBox>

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateProjectDialog)
{
    ui->setupUi(this);
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

void CreateProjectDialog::on_pushButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("打开文件夹"));
    fileDialog->setFileMode(QFileDialog::Directory);
    if(fileDialog->exec() == QDialog::Accepted) {
                   QString path = fileDialog->selectedFiles()[0];
                   ui->lineEdit_2->setText(path);
           }

}
