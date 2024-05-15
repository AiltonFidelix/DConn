#ifndef WSLCONN_H
#define WSLCONN_H

#include <QMainWindow>
#include "wslinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WSLConn;
}
QT_END_NAMESPACE

class WSLConn : public QMainWindow
{
    Q_OBJECT

public:
    WSLConn(QWidget *parent = nullptr);
    ~WSLConn();

private slots:
    void getDistros();
    void setDistros(QStringList distros);

private:
    Ui::WSLConn *ui;
    WSLInterface m_interface;
};
#endif // WSLCONN_H
