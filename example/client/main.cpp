#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("LoL Replay Uploader");
    a.setApplicationName("LoL Replay Uploader");

    MainWindow w;
    w.show();

    return a.exec();
}
