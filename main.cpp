#include "mapviz.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapViz w;
    w.show();

    return a.exec();
}
