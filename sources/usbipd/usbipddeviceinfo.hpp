#ifndef USBIPDDEVICEINFO_H
#define USBIPDDEVICEINFO_H

#include <QList>
#include <QString>

#include "usbipdstateenum.hpp"

struct USBIPDDeviceInfo {
    USBIPDStateEnum state;
    QString device;
    QString busId;
};

using USBIPDDeviceInfoList = QList<USBIPDDeviceInfo>;

#endif  // USBIPDDEVICEINFO_H
