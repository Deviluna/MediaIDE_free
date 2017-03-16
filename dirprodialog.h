#ifndef DIRPRODIALOG_H
#define DIRPRODIALOG_H

#include <QDialog>

namespace Ui {
class DirProDialog;
}

class DirProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DirProDialog(QWidget *parent = 0);
    ~DirProDialog();
    void setPath(QString path);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DirProDialog *ui;
    QString dirProPath;

};

#endif // DIRPRODIALOG_H
