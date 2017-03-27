#include "introdialog.h"
#include "ui_introdialog.h"
#include <testwidget.h>
#include <mainwindow.h>
#include <argall.h>

IntroDialog::IntroDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntroDialog)
{
    ui->setupUi(this);
}

IntroDialog::~IntroDialog()
{
    delete ui;
}
void IntroDialog::setIntroPic(QString path){
    ui->lineEdit->setText(path);
}
void IntroDialog::setIntroText(QString intro){
    ui->textEdit->setText(intro);
}

void IntroDialog::on_pushButton_2_clicked()
{
    TestWidget *tw=(TestWidget*)parentWidget();
    tw->mList[4]=ui->lineEdit->text();
    tw->mList[5]=ui->textEdit->toPlainText();
    //小问题，保存简介同时要不要保存文章
    tw->save();
    close();
}

void IntroDialog::on_pushButton_3_clicked()
{
    close();
}

void IntroDialog::on_pushButton_clicked()
{
    QString path=ArgAll::insertImage(this);
    ui->lineEdit->setText(path);

}
