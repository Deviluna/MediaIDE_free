#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QString>


namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);

    void loadFile(QString path);

    bool changed();

    QString getTitle();

    void save();

    ~TestWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_7_clicked();

    void on_toolButton_8_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_9_clicked();

    void on_toolButton_11_clicked();

    void on_toolButton_10_clicked();

    void on_toolButton_12_clicked();

    void on_toolButton_13_clicked();

    void on_textEdit_textChanged();


private:
    Ui::TestWidget *ui;
    void outputFile();
    void setBold();
    void setBigger();
    void setSmaller();
    void cleanText();
    void loadProject(QString path);
    void addFile(QString fileName);
    void previewHtml(QString path);
    //void loadFile(QString path);
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
    QString nowText;
    QString nowFile;
    QString rootPath;
    QString nowPath;
    QTimer *timer;
    QFont quickFont;
    QColor quickColor;
    bool change;
};

#endif // TESTWIDGET_H
