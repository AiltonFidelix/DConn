#include <QApplication>
#include <QIcon>

#include "dconn.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DConn w;

    w.setWindowIcon(QIcon(QStringLiteral(":/icons/icon.svg")));
    w.start();

    return a.exec();
}
