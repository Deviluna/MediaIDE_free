#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QFileSystemModel>
#include <QMenu>
#include <QWidget>
#include <QCloseEvent>
#include <QJsonArray>
#include <QJsonObject>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initProgramme();
    void CreateProject();
    void OpenProject();
    void CloseNowTab();
    void renameNowDir(QString newName);
    QWidget* Page();
    bool renameNowTab(QString newName);
private slots:

    void createProject();

    void renameDir();

    void closeTab(int a);

    void on_pushButton_clicked();

    void about();

    void selectFile(const QModelIndex &index);

    void addArticle();

    void addDir();

    void on_pushButton_7_clicked();

    void update();

    void openProject();

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_8_clicked();

    void deleteFile();

    void deleteDir();

    void generate();

    void openInExplorer();


    void on_pushButton_2_clicked();

    void on_tabWidget_customContextMenuRequested(const QPoint &pos);

    void on_tabWidget_tabBarClicked(int index);

private:
    void setupMenu();
    void closeAllTab();
    void firstUse();
    void initTabWidget();
    void setBold();
    void setBigger();
    void setSmaller();
    void refreshTree();
    void cleanText();
    void loadProject(QString path);
    void addFile(QString fileName);
    void loadFile(QString path);
    void outputFile(QString path);
    void setTreeview();
    void setColor(const QColor &c);
    void setFont(const QFont &font);
    void setUnderline();
    void getNowtext();
    void outputText();
    void setQuickFont();
    void setQuickColor();
    void setAlign(Qt::Alignment align);
    void setMenuAction();
    void insertImage();
    void loadJson();
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QString nowText;
    QString nowFile;
    QString rootPath;
    QString nowPath;
    QMenu *m_folderMenu;
    QMenu *m_fileMenu;
    QTimer *timer;
    QFont quickFont;
    QColor quickColor;
    QModelIndex nowIndex;
    QString prootPath;
    QString mstPath;
    QStringList mstList;
    QJsonObject mstJsonObject;
    void closeEvent(QCloseEvent *event);
    void setDirPro(QString Path);

};

#endif // MAINWINDOW_H
