#include "addfiledialog.h"
#include "ui_addfiledialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

AddFileDialog::AddFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFileDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->lineEdit_2->text().length()>0);
}

AddFileDialog::~AddFileDialog()
{
    delete ui;
}

void AddFileDialog::on_buttonBox_accepted()
{

}

void AddFileDialog::on_lineEdit_2_textChanged(const QString &arg1)
{

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->lineEdit_2->text().length()>0);
}

QString AddFileDialog::getinput(){
    return ui->lineEdit_2->text();
}
