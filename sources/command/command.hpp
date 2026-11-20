#ifndef COMMAND_H
#define COMMAND_H

class Command {
public:
    virtual ~Command() = default;

    virtual void run() = 0;
};

#endif  // COMMAND_H
