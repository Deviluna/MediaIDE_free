#include "seturldialog.h"
#include "ui_seturldialog.h"
#include <testwidget.h>


SetURLDialog::SetURLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetURLDialog)
{
    ui->setupUi(this);
}

SetURLDialog::~SetURLDialog()
{
    delete ui;
}

void SetURLDialog::on_pushButton_clicked()
{

    TestWidget *tw=(TestWidget*)parentWidget();
    tw->setURL(ui->lineEdit->text());
    close();
}

void SetURLDialog::on_pushButton_2_clicked()
{
    close();
}

void SetURLDialog::loadUrl(QString url){
    ui->lineEdit->setText(url);

}
