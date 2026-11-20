#include "usbipdstatehelper.hpp"

#include <QMap>

static QMap<USBIPDStateEnum, QString> STATE_TO_STRING_MAP = {
    {USBIPDStateEnum::Unknown, QStringLiteral("Unknown")},
    {USBIPDStateEnum::NotShared, QStringLiteral("Not shared")},
    {USBIPDStateEnum::Shared, QStringLiteral("Shared")},
    {USBIPDStateEnum::Attached, QStringLiteral("Attached")},
};

static QMap<QString, USBIPDStateEnum> SRING_TO_STATE_MAP = {
    {QStringLiteral("Unknown"), USBIPDStateEnum::Unknown},
    {QStringLiteral("Not shared"), USBIPDStateEnum::NotShared},
    {QStringLiteral("Shared"), USBIPDStateEnum::Shared},
    {QStringLiteral("Attached"), USBIPDStateEnum::Attached},
};

QString USBIPDStateHelper::stateToString(const USBIPDStateEnum state) {
    return STATE_TO_STRING_MAP.value(state, QStringLiteral("Unknown"));
}

USBIPDStateEnum USBIPDStateHelper::stringToState(const QString &state) {
    return SRING_TO_STATE_MAP.value(state, USBIPDStateEnum::Unknown);
}
