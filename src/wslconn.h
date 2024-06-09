#ifndef WSLCONN_H
#define WSLCONN_H

#include <QMainWindow>
#include "wslhandle.h"

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

protected:
    void showEvent(QShowEvent *event);

private slots:
    void setDistros(QJsonObject json);
    void setUSBPorts(QHash<QString, QStringList> ports);
    void about();

private:
    void updateStatus();
    void attachPort();

private:
    Ui::WSLConn *ui;
    WSLHandle m_handle;
    OPFactory m_factory;
};

#endif // WSLCONN_H
