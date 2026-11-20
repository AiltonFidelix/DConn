#ifndef USBIPD_H
#define USBIPD_H

#include <QString>

#include "usbipdstateenum.hpp"

class USBIPD {
public:
    bool verifyState(const QString &busId, const USBIPDStateEnum state) const;
};

#endif  // USBIPD_H
