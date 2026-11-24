#ifndef COMMANDVERSION_H
#define COMMANDVERSION_H

#include <QVersionNumber>

#include "command.hpp"
#include "commandobserver.hpp"

class CommandVersion : public Command {
public:
    explicit CommandVersion(CommandObserver *observer);

    void run() override;

private:
    CommandObserver *_observer;

    QVersionNumber parseVersion(const QByteArray &output);
    void notifyError(const QString &error);
};

#endif  // COMMANDVERSION_H
