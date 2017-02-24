#ifndef ADDDIRDIALOG_H
#define ADDDIRDIALOG_H

#include <QDialog>

namespace Ui {
class addDirDialog;
}

class addDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDirDialog(QWidget *parent = 0);
    ~addDirDialog();
    QString getinput();

private slots:
    void on_buttonBox_accepted();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::addDirDialog *ui;
};

#endif // ADDDIRDIALOG_H
