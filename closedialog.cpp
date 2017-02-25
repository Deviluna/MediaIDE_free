#include "closedialog.h"
#include "ui_closedialog.h"
#include <mainwindow.h>
#include <testwidget.h>


CloseDialog::CloseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloseDialog)
{
    ui->setupUi(this);
}

CloseDialog::~CloseDialog()
{
    delete ui;
}

void CloseDialog::on_pushButton_clicked()
{
    hide();
    MainWindow *mainWindow=(MainWindow*)parentWidget();

    TestWidget *tw=(TestWidget*)mainWindow->Page();
    tw->save();
    mainWindow->CloseNowTab();

}

void CloseDialog::on_pushButton_3_clicked()
{
    hide();
}

void CloseDialog::on_pushButton_2_clicked()
{
    //子窗口调用父窗口ji经典案例
    hide();
    MainWindow *mainWindow=(MainWindow*)parentWidget();
    mainWindow->CloseNowTab();
}
