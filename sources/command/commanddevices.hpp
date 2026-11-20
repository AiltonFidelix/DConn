#ifndef OPUSBPorts_H
#define OPUSBPorts_H

#include "command.hpp"
#include "commandobserver.hpp"

class CommandDevices : public Command {
public:
    explicit CommandDevices(CommandObserver *observer);

    void run() override;

private:
    CommandObserver *_observer;

    void notifyError(const QString &error);
};

#endif // OPUSBPorts_H
