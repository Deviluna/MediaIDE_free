#ifndef RMDIRDIALOG_H
#define RMDIRDIALOG_H

#include <QDialog>

namespace Ui {
class RmdirDialog;
}

class RmdirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RmdirDialog(QWidget *parent = 0);
    ~RmdirDialog();

private:
    Ui::RmdirDialog *ui;
};

#endif // RMDIRDIALOG_H
