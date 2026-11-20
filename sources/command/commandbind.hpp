#ifndef COMMANDBIND_H
#define COMMANDBIND_H

#include "command.hpp"
#include "commandobserver.hpp"

class CommandBind : public Command {
public:
    explicit CommandBind(CommandObserver *observer, const QString &busId);

    void run() override;

private:
    CommandObserver *_observer;
    QString _busId;

    void notifyError(const QString &error);
};

#endif  // COMMANDBIND_H
