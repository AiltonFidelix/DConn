#include "opattach.h"

#include <QMessageBox>

OPAttach::OPAttach(QObject *parent)
{
    m_isComplete = false;
}

void
OPAttach::runCommand()
{
    if (m_args.count() == 2)
    {

        QString command = QString("usbipd attach --wsl %1 --busid %2\r\n").arg(m_args.at(0), m_args.at(1));
        m_process->write(command.toUtf8());

        m_isComplete = true;

        emit attached();
    }
    else
    {
        QMessageBox::warning(nullptr, "Attach", "Not valid arguments!");
    }
}

void
OPAttach::setCommandArgs(QStringList args)
{
    m_args = args;
}

void
OPAttach::setProcess(QProcess *process)
{
    m_process = process;
}

void
OPAttach::parser(QString data)
{
    Q_UNUSED(data)
}

bool
OPAttach::isComplete()
{
    return m_isComplete;
}
