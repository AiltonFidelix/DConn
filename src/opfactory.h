#ifndef OPFACTORY_H
#define OPFACTORY_H

#include <QObject>

#include "ioperation.h"

class OPFactory : public QObject
{
    Q_OBJECT
public:
    enum Operation { Attach, Distros, USBPorts };

    explicit OPFactory(QObject *parent = nullptr);

    void setProcess(QProcess *process);

    iOperation *create(Operation operation, QStringList args = QStringList());

private:
    QProcess *m_process;
};

#endif // OPFACTORY_H
