#ifndef OPDETACH_H
#define OPDETACH_H

#include "ioperation.h"

#include <QObject>

class OPDetach : public QObject, public iOperation
{
    Q_OBJECT
public:
    explicit OPDetach(QObject *parent = nullptr);

    void runCommand();
    void setCommandArgs(QStringList args);
    void setProcess(QProcess *process);
    void parser(QString data);
    bool isComplete();

signals:
    void detached();

private:
    QProcess *m_process;
    QStringList m_args;
    bool m_isComplete;
};

#endif // OPDETACH_H
