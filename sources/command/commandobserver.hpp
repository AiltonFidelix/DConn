#ifndef COMMANDOBSERVER_H
#define COMMANDOBSERVER_H

#include "usbipddeviceinfo.hpp"

class CommandObserver {
public:
    virtual ~CommandObserver() = default;

    virtual void notifyAttached(const QString &busId) = 0;
    virtual void notifyBound(const QString &busId) = 0;
    virtual void notifyDetached(const QString &busId) = 0;
    virtual void notifyDevices(const USBIPDDeviceInfoList &devices) = 0;
    virtual void notifyError(const QString &error) = 0;
    virtual void notifySystemReady() = 0;
    virtual void notifyNotReady() = 0;
};

#endif  // COMMANDOBSERVER_H
