#include "opusbports.h"

#include <QJsonArray>
#include <QJsonValue>

OPUSBPorts::OPUSBPorts(QObject *parent)
    : QObject{parent}
{
    m_isComplete = false;
}

void
OPUSBPorts::runCommand()
{
    m_process->write("usbipd list\r\n");
}

void
OPUSBPorts::setCommandArgs(QStringList args)
{
    Q_UNUSED(args)
}

void
OPUSBPorts::setProcess(QProcess *process)
{
    m_process = process;
}

void
OPUSBPorts::parser(QString data)
{
    QStringList lines = data.trimmed().split("\r\n");

    static QRegularExpression regex("\\s{2,}");

    QJsonArray array;

    for (int i = 2; i < lines.count(); ++i)
    {
        QStringList info = lines.at(i).trimmed().split(regex);

        if (info.count() >= 3)
        {
            QJsonArray tmpArray;
            QJsonValue value;
            value = info.at(2);
            tmpArray.append(value);
            value = info.at(0);
            tmpArray.append(value);
            value = info.at(3);
            tmpArray.append(value);
            array.append(tmpArray);
        }
    }

    if (array.count() > 0)
    {
        QJsonObject json;
        json.insert("ports", array);

        emit usbPorts(json);

        m_isComplete = true;
    }
}

bool
OPUSBPorts::isComplete()
{
    return m_isComplete;
}
