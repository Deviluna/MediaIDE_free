#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWidget>

namespace Ui {
class TestPage;
}

class TestPage : public QWidget
{
    Q_OBJECT

public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestPage *ui;
    void test();
    QString loadJson(QString path);
};

#endif // TESTPAGE_H
