#ifndef IOPERATION_H
#define IOPERATION_H

#include <QStringList>
#include <QProcess>
#include <QJsonObject>

class iOperation
{
public:
    virtual void runCommand() = 0;
    virtual void setCommandArgs(QStringList args) = 0;
    virtual void setProcess(QProcess *process) = 0;
    virtual void parser(QString data) = 0;
    virtual bool isComplete() = 0;

    virtual ~iOperation() {};
};

#endif // IOPERATION_H
