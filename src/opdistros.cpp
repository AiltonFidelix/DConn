#include "opdistros.h"

#include <QJsonArray>

OPDistros::OPDistros(QObject *parent)
    : QObject(parent)
{
    m_isComplete = false;
}

void
OPDistros::runCommand()
{
    m_process->write("wsl --list\r\n");
}

void
OPDistros::setCommandArgs(QStringList args)
{
    Q_UNUSED(args)
}

void
OPDistros::setProcess(QProcess *process)
{
    m_process = process;
}

void
OPDistros::parser(QString data)
{
    QStringList lines = data.trimmed().split("\r\n");
    QStringList distrosList;

    for (int i = 1; i < lines.count(); ++i)
    {
        distrosList.append(lines.at(i));
    }

    if (distrosList.count() > 0)
    {
        QJsonArray array;

        for (const QString &distro : distrosList)
        {
            array.append(distro);
        }

        QJsonObject json;
        json.insert("distros", array);

        emit distros(json);

        m_isComplete = true;
    }
}

bool
OPDistros::isComplete()
{
    return m_isComplete;
}
