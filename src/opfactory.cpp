#include "opfactory.h"
#include "opdistros.h"

OPFactory::OPFactory(QObject *parent)
    : QObject{parent}
{}

void
OPFactory::setProcess(QProcess *process)
{
    m_process = process;
}

iOperation*
OPFactory::create(Operation operation, QStringList args)
{
    iOperation *obj = nullptr;

    switch (operation)
    {
        case Attach:
            break;
        case Distros:
            obj = new OPDistros();
            break;
        case USBPorts:
            break;
    }

    if (obj != nullptr)
    {
        obj->setProcess(m_process);

        if (args.count() > 0)
        {
            obj->setCommandArgs(args);
        }
    }

    return obj;
}
