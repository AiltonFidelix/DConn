#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <expected>
#include <memory>

#include "command.hpp"
#include "commandattach.hpp"
#include "commandbind.hpp"
#include "commanddetach.hpp"
#include "commanddevices.hpp"
#include "commandenum.hpp"
#include "commandversion.hpp"

class CommandFactory {
public:
    CommandFactory() = delete;

    using Result = std::expected<std::unique_ptr<Command>, std::string>;

    template <typename... Args>
    static std::expected<std::unique_ptr<Command>, std::string> create(const CommandEnum cmd, Args... args) {
        switch (cmd) {
            case CommandEnum::AttachPort:
                return std::make_unique<CommandAttach>(args...);
            case CommandEnum::BindPort:
                return std::make_unique<CommandBind>(args...);
            case CommandEnum::DetachPort:
                return std::make_unique<CommandDetach>(args...);
            default:
                return std::unexpected("Invalid command");
        }
    }

    template <typename Arg>
    static std::expected<std::unique_ptr<Command>, std::string> create(const CommandEnum cmd, Arg arg) {
        switch (cmd) {
            case CommandEnum::ListDevices:
                return std::make_unique<CommandDevices>(arg);
            case CommandEnum::Version:
                return std::make_unique<CommandVersion>(arg);
            default:
                return std::unexpected("Invalid command");
        }
    }
};

#endif  // COMMANDFACTORY_H
