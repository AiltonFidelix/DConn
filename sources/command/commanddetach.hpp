#ifndef COMMANDDETACH_H
#define COMMANDDETACH_H

#include "command.hpp"
#include "commandobserver.hpp"

class CommandDetach : public Command {
public:
    explicit CommandDetach(CommandObserver *observer, const QString &busId);

    void run() override;

private:
    CommandObserver *_observer;
    QString _busId;

    void notifyError(const QString &error);
};

#endif // COMMANDDETACH_H
