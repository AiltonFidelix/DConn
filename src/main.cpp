#include "wslconn.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WSLConn w;

    w.setWindowIcon(QIcon(":/icons/icon.png"));
    w.show();

    return a.exec();
}
