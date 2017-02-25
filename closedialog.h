#ifndef CLOSEDIALOG_H
#define CLOSEDIALOG_H

#include <QDialog>

namespace Ui {
class CloseDialog;
}

class CloseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CloseDialog(QWidget *parent = 0);
    ~CloseDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CloseDialog *ui;
};

#endif // CLOSEDIALOG_H
