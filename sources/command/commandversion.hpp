#ifndef COMMANDVERSION_H
#define COMMANDVERSION_H

#include "command.hpp"
#include "commandobserver.hpp"

class CommandVersion : public Command {
public:
    explicit CommandVersion(CommandObserver *observer);

    void run() override;

private:
    CommandObserver *_observer;

    void notifyError(const QString &error);
};

#endif  // COMMANDVERSION_H
