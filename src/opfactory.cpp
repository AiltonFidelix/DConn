#include "opfactory.h"
#include "opattach.h"
#include "opdetach.h"
#include "opdistros.h"
#include "opusbports.h"

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
            obj = new OPAttach();
            break;
        case Detach:
            obj = new OPDetach();
            break;
        case Distros:
            obj = new OPDistros();
            break;
        case USBPorts:
            obj = new OPUSBPorts();
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
