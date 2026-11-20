#ifndef USBIPDSTATEENUM_H
#define USBIPDSTATEENUM_H

#include <cstdint>

enum class USBIPDStateEnum : uint8_t {
    Unknown = 0,
    NotShared,
    Shared,
    Attached,
};

#endif  // USBIPDSTATEENUM_H
