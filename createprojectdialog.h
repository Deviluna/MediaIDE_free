#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class CreateProjectDialog;
}

class CreateProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateProjectDialog(QWidget *parent = 0);
    ~CreateProjectDialog();
    QString getProjectName();
    QString getProjectPath();
    void firstUse();


private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateProjectDialog *ui;
};

#endif
