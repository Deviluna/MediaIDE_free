#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

#include <QDialog>
#include <testwidget.h>

namespace Ui {
class GenerateDialog;
}

class GenerateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateDialog(QWidget *parent = 0);
    ~GenerateDialog();
    void setProjectPath(QString path);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::GenerateDialog *ui;
    QString projectPath;
    void genHtml(QString path,QString dirPath);
    TestWidget *tw;
    void genDir(QString path,QString dirPath);
    void genIndex();

    QString indexlist;
};

#endif // GENERATEDIALOG_H
