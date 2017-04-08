#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QString>
#include <QTextCharFormat>
#include <QJsonObject>


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

    void setRootpath(QString path);

    QString getTitle();

    void save();

    QString getPath();

    void findWord(QString word);

    void setURL(QString URL);

    ~TestWidget();
    void outputFile();
    void setBold();
    void setBigger();
    void setSmaller();
    void cleanText();
    void loadProject(QString path);
    void addFile(QString fileName);
    void previewHtml(QString path);
    //void loadFile(QString path);
    bool outputFile(QString path);
    bool newFile(QString path);
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
    QString replaceTemplate(QString temp,QString title,QString author,QString date,QString content);
    QString nowText;
    QString nowFile;
    QString rootPath;
    QString nowPath;
    QTimer *timer;
    QFont quickFont;
    QColor quickColor;
    bool change;
    QString preText;
    QStringList mList;
    QTextCharFormat clearFormat;
    QTextCharFormat selectedFormat;
    QJsonObject mJsonObject;

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


    void on_toolButton_14_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_toolButton_15_clicked();

    void on_toolButton_16_clicked();

    void on_toolButton_21_clicked();

    void on_toolButton_19_clicked();

    void on_toolButton_20_clicked();

    void on_toolButton_20_toggled(bool checked);

private:
    Ui::TestWidget *ui;

};

#endif // TESTWIDGET_H
