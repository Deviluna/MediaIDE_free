#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void CreateProject();

    void on_pushButton_4_clicked();

    void on_commitButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void about();

    void selectFile(const QModelIndex &index);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void update();

    void openProject();
private:
    QTimer *timer;
    void loadProject(QString path);
    void addFile(QString fileName);
    void previewHtml(QString path);
    void loadFile(QString path);
    void outputFile(QString path);
    void setTreeview();
    void setColor(const QColor &c);
    void setFont();
    void setUnderline();
    void getNowtext();
    void outputText();
    void setAlign(Qt::Alignment align);
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QString nowText;
    QString nowFile;
    QString rootPath;
};

#endif // MAINWINDOW_H
