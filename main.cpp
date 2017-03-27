#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QElapsedTimer>
#include <QSplitter>
#include <qtextcodec.h>
#include <QString>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash=new QSplashScreen;
    MainWindow w;
    splash->setPixmap(QPixmap(":/new/prefix1/Images/log.png"));
    splash->show();
    splash->showMessage("加载核心资源...", Qt::AlignBottom|Qt::AlignRight, Qt::black);
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<500)
    {
        QCoreApplication::processEvents();
    }
    splash->showMessage("设置用户界面...", Qt::AlignBottom|Qt::AlignRight, Qt::black);
    w.setWindowTitle("Media IDE Beta");
    w.show();
    splash->finish(&w);
    w.initProgramme();
    return a.exec();
}
//重要，对文件名要有正则表达式限定


