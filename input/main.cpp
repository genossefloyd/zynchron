#include "maingui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainGui w;
    w.show();

    return a.exec();
}
