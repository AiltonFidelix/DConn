#ifndef DCONN_H
#define DCONN_H

#include <QMainWindow>

#include "commandservice.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
    class DConn;
}
QT_END_NAMESPACE

class DConn : public QMainWindow {
    Q_OBJECT
public:
    DConn(QWidget *parent = nullptr);
    ~DConn();

    void start();

private slots:
    void onAbout();
    void onAction();
    void onRefresh();
    void onAttached(const QString &busId);
    void onDetached(const QString &busId);
    void onDevicesReady(const USBIPDDeviceInfoList &devices);
    void onSelectDevice();
    void onErrorOccurred(const QString &error);
    void onShowDevices();
    void onNotReady();

private:
    void enableActions();
    void disableActions();

    QString stateDescriptionFromState(const USBIPDStateEnum state);
    QString actionDescriptionFromState(const USBIPDStateEnum state);

private:
    Ui::DConn *_ui;
    CommandService _service;
};

#endif  // DCONN_H
