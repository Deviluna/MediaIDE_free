#include "adddirdialog.h"
#include "ui_adddirdialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

addDirDialog::addDirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDirDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->lineEdit->text().length()>0);
}

addDirDialog::~addDirDialog()
{
    delete ui;
}

void addDirDialog::on_buttonBox_accepted()
{
}

void addDirDialog::on_lineEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->lineEdit->text().length()>0);

}

QString addDirDialog::getinput(){
    return ui->lineEdit->text();
}
