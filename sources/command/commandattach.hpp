#ifndef COMMANDATTACH_H
#define COMMANDATTACH_H

#include "command.hpp"
#include "commandobserver.hpp"
#include "usbipd.hpp"

class CommandAttach : public Command, public USBIPD {
public:
    explicit CommandAttach(CommandObserver *observer, const QString &busId);

    void run() override;

private:
    CommandObserver *_observer;
    QString _busId;

    void notifyError(const QString &error);
};

#endif // COMMANDATTACH_H
