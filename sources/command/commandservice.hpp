#ifndef COMMANDSERVICE_H
#define COMMANDSERVICE_H

#include <QObject>
#include <QProcess>

#include "commandenum.hpp"
#include "commandobserver.hpp"
#include "usbipdstateenum.hpp"

class CommandService : public QObject, public CommandObserver {
    Q_OBJECT
public:
    explicit CommandService(QObject *parent = nullptr);
    ~CommandService() override = default;

    void verifySystem();
    void showDevices();

    void attach(const USBIPDDeviceInfo &device);
    void detach(const USBIPDDeviceInfo &device);

    void notifyAttached(const QString &busId) override;
    void notifyBound(const QString &busId) override;
    void notifyDetached(const QString &busId) override;
    void notifyDevices(const USBIPDDeviceInfoList &devices) override;
    void notifyError(const QString &error) override;
    void notifySystemReady() override;
    void notifyNotReady() override;

signals:
    void devicesReady(const USBIPDDeviceInfoList &devices);
    void deviceAttached(const QString &busId);
    void deviceDetached(const QString &busId);
    void errorOccurred(const QString &error);
    void systemReady();
    void notReady();

private:
    void runCommand(const QString &busId, const USBIPDStateEnum state);
    void runCommand(const CommandEnum command);
    CommandEnum stateToCommand(const USBIPDStateEnum state) const;
};

#endif // COMMANDSERVICE_H
