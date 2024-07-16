#ifndef OPATTACH_H
#define OPATTACH_H

#include "ioperation.h"

#include <QObject>

class OPAttach : public QObject, public iOperation
{
    Q_OBJECT
public:
    explicit OPAttach(QObject *parent = nullptr);

    void runCommand();
    void setCommandArgs(QStringList args);
    void setProcess(QProcess *process);
    void parser(QString data);
    bool isComplete();

signals:
    void attached();

private:
    QProcess *m_process;
    QStringList m_args;
    bool m_isComplete;
};

#endif // OPATTACH_H
