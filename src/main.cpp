#include "wslconn.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WSLConn w;
    w.show();

    return a.exec();
}
