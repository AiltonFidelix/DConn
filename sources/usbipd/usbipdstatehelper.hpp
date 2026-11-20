#ifndef USBIPDSTATEHELPER_H
#define USBIPDSTATEHELPER_H

#include <QString>

#include "usbipdstateenum.hpp"

class USBIPDStateHelper {
public:
    USBIPDStateHelper() = delete;

    static QString stateToString(const USBIPDStateEnum state);
    static USBIPDStateEnum stringToState(const QString &state);
};

#endif  // USBIPDSTATEHELPER_H
