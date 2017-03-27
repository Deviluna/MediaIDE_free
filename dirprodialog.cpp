#include "dirprodialog.h"
#include "ui_dirprodialog.h"
#include <QString>
#include <argall.h>


DirProDialog::DirProDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DirProDialog)
{
    ui->setupUi(this);

}

DirProDialog::~DirProDialog()
{
    delete ui;
}

void DirProDialog::on_pushButton_2_clicked()
{
    close();
}

void DirProDialog::setPath(QString path){
    dirProPath=path;
    ui->lineEdit->setText(ArgAll::getValueJson(path,"order"));
}

void DirProDialog::on_pushButton_clicked()
{
    //确认修改分类的属性
    QString sortDigit=ui->lineEdit->text();
    ArgAll::modifyJson(dirProPath,"order",sortDigit);
    close();
}
