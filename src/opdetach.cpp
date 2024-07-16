#include "opdetach.h"

#include <QMessageBox>

OPDetach::OPDetach(QObject *parent)
{
    m_isComplete = false;
}

void
OPDetach::runCommand()
{
    if (m_args.count() == 2)
    {
        QString command = QString("usbipd detach --busid %1\r\n").arg(m_args.at(1));
        m_process->write(command.toUtf8());

        m_isComplete = true;

        emit detached();
    }
    else
    {
        QMessageBox::warning(nullptr, "Detach", "Not valid arguments!");
    }
}

void
OPDetach::setCommandArgs(QStringList args)
{
    m_args = args;
}

void
OPDetach::setProcess(QProcess *process)
{
    m_process = process;
}

void
OPDetach::parser(QString data)
{
    Q_UNUSED(data)
}

bool
OPDetach::isComplete()
{
    return m_isComplete;
}
