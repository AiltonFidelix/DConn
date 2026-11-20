#include "commandservice.hpp"

#include <QDebug>

#include "command.hpp"
#include "commandfactory.hpp"

CommandService::CommandService(QObject *parent)
    : QObject{parent} {
}

void CommandService::verifySystem() {

    qInfo() << "CommandService::verifySystem";

    runCommand(CommandEnum::Version);
}

void CommandService::showDevices() {

    qInfo() << "CommandService::showDevices";

    runCommand(CommandEnum::ListDevices);
}

void CommandService::attach(const USBIPDDeviceInfo &device) {

    qInfo() << "CommandService::attach [BUS_ID]" << device.busId;

    runCommand(device.busId, device.state);
}

void CommandService::detach(const USBIPDDeviceInfo &device) {

    qInfo() << "CommandService::detach [BUS_ID]" << device.busId;

    runCommand(device.busId, device.state);
}

void CommandService::notifyAttached(const QString &busId) {
    emit deviceAttached(busId);
}

void CommandService::notifyBound(const QString &busId) {

    qInfo() << "CommandService::notifyBound [BUS_ID]" << busId;

    runCommand(busId, USBIPDStateEnum::Shared);
}

void CommandService::notifyDetached(const QString &busId) {
    emit deviceDetached(busId);
}

void CommandService::notifyDevices(const USBIPDDeviceInfoList &devices) {
    emit devicesReady(devices);
}

void CommandService::notifyError(const QString &error) {

    qWarning() << "CommandService::notifyError [ERROR]" << error;

    emit errorOccurred(error);
}

void CommandService::notifySystemReady() {
    emit systemReady();
}

void CommandService::notifyNotReady() {
    emit notReady();
}

void CommandService::runCommand(const QString &busId, const USBIPDStateEnum state) {

    qInfo() << "CommandService::runCommand [BUS_ID]" << busId << "[STATE]" << static_cast<uint8_t>(state);

    CommandFactory::Result result = CommandFactory::create(stateToCommand(state), this, busId);

    if (!result.has_value()) {
        qWarning() << "CommandService::runCommand [ERROR]" << result.error();
        return;
    }

    result.value()->run();
}

void CommandService::runCommand(const CommandEnum command) {

    qInfo() << "CommandService::runCommand [COMMAND]" << static_cast<uint8_t>(command);

    CommandFactory::Result result = CommandFactory::create(command, this);

    if (!result.has_value()) {
        qWarning() << "CommandService::runCommand [ERROR]" << result.error();
        return;
    }

    result.value()->run();
}

CommandEnum CommandService::stateToCommand(const USBIPDStateEnum state) const {
    switch (state) {
        case USBIPDStateEnum::Attached:
            return CommandEnum::DetachPort;
        case USBIPDStateEnum::NotShared:
            return CommandEnum::BindPort;
        case USBIPDStateEnum::Shared:
            return CommandEnum::AttachPort;
        default:
            return CommandEnum::Unknown;
    }
}