#ifndef COMMANDENUM_H
#define COMMANDENUM_H

#include <cstdint>

enum class CommandEnum : uint8_t {
    Unknown = 0,
    AttachPort,
    BindPort,
    DetachPort,
    ListDevices,
    Version,
};

#endif  // COMMANDENUM_H
