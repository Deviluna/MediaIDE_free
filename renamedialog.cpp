#include "renamedialog.h"
#include "ui_renamedialog.h"
#include <mainwindow.h>

RenameDialog::RenameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDialog)
{
    ui->setupUi(this);
}

RenameDialog::~RenameDialog()
{
    delete ui;
}

void RenameDialog::setNowName(QString fileName){
    ui->lineEdit->setText(fileName);
}

void RenameDialog::on_pushButton_2_clicked()
{
    close();
}

void RenameDialog::on_pushButton_clicked()
{
    MainWindow *mw=(MainWindow*)parentWidget();
    mw->renameNowDir(ui->lineEdit->text());
    close();
}
