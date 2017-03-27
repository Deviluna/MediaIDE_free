#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include <mainwindow.h>
#include <QDesktopServices>
#include <QUrl>
WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    setWindowTitle("欢迎使用Media IDE");
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::on_commandLinkButton_clicked()
{
    hide();
    MainWindow *ptr = (MainWindow*)parentWidget();
    ptr->CreateProject();
}

void WelcomeDialog::on_commandLinkButton_2_clicked()
{
    hide();
    MainWindow *ptr = (MainWindow*)parentWidget();
    ptr->OpenProject();
}

void WelcomeDialog::on_commandLinkButton_3_clicked()
{
    QUrl url("http://deviluna.com");
    QDesktopServices::openUrl(url);

}
