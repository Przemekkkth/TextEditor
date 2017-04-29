#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/WindowIcon/png/WindowIcon/text-editor.png"));
    splash->setGeometry(0, 0, 512, 512);
    splash->move(100,100);
    splash->show();



    MainWindow *w = new MainWindow;
    w->setGeometry(0, 0, 512, 512);
    w->move(100, 100);
    QTimer::singleShot(1000, splash, SLOT(close()));
    QTimer::singleShot(1000, w, SLOT(show()));


    //w.show();

    return a.exec();
}
