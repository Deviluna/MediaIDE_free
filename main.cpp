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
      QElapsedTimer t;
        t.start();

        while(t.elapsed()<1000)
        {
            QCoreApplication::processEvents();
        }
        w.setWindowTitle("Media IDE Beta");
        w.show();
        splash->finish(&w);
        w.initProgramme();

    return a.exec();
}



