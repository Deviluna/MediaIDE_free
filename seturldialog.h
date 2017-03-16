#ifndef SETURLDIALOG_H
#define SETURLDIALOG_H

#include <QDialog>

namespace Ui {
class SetURLDialog;
}

class SetURLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetURLDialog(QWidget *parent = 0);
    ~SetURLDialog();
    void loadUrl(QString url);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SetURLDialog *ui;
};

#endif // SETURLDIALOG_H
