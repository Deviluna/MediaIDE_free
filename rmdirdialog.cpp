#include "rmdirdialog.h"
#include "ui_rmdirdialog.h"

RmdirDialog::RmdirDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RmdirDialog)
{
    ui->setupUi(this);
}

RmdirDialog::~RmdirDialog()
{
    delete ui;
}
