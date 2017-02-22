#ifndef ADDFILEDIALOG_H
#define ADDFILEDIALOG_H

#include <QDialog>

namespace Ui {
class AddFileDialog;
}

class AddFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFileDialog(QWidget *parent = 0);
    ~AddFileDialog();
    QString getinput();

private slots:
    void on_buttonBox_accepted();

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::AddFileDialog *ui;
};

#endif // ADDFILEDIALOG_H
