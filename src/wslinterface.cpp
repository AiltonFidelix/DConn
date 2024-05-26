#include "wslinterface.h"

#include <QCoreApplication>
#include <QDebug>

WSLInterface::WSLInterface(QObject *parent)
    : QObject(parent)
{
    m_operation = Initializing;
    connect(&m_process, &QProcess::started, this, &WSLInterface::started);
    connect(&m_process, &QProcess::readyRead, this, &WSLInterface::readyRead);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &WSLInterface::readyReadStandardOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &WSLInterface::readyReadStandardError);
}

WSLInterface::~WSLInterface()
{
    m_process.close();
}

void
WSLInterface::start()
{
    m_process.start("cmd", QStringList());
}

void
WSLInterface::nextOperation(Operation operation)
{
    if ((m_operation == None)
        && (m_operations.count() == 0)
        && (m_operation != Initializing))
    {
        execOperation(operation);
    }
    else
    {
        m_operations.append(operation);
    }
}

void
WSLInterface::execOperation(Operation operation)
{
    if (!m_process.isWritable())
    {
        return;
    }

    m_operation = operation;

    switch (operation)
    {
        case Distros:
            m_process.write("wsl --list\r\n");
            break;
        case USBPorts:
            // m_process.write("wsl --list\r\n");
            break;
        default:
            break;
    }
}

void
WSLInterface::parserDistros(QString input)
{
    QStringList lines = input.trimmed().split("\r\n");
    QStringList distrosList;

    for (int i = 1; i < lines.count(); ++i)
    {
        distrosList.append(lines.at(i));
    }

    if (distrosList.count() > 0)
    {
        emit distros(distrosList);

        m_operation = None;
    }
}

void
WSLInterface::parserUSBPorts(QString input)
{
#warning // TODO: implent this parser
    QStringList lines = input.trimmed().split("\r\n");
}

void
WSLInterface::started()
{
    emit ready();
}

void
WSLInterface::readyRead()
{
    QString output = QString::fromLatin1(m_process.readAll());
    output.replace('\0',R"(\u0000)");
    output.replace("\\u0000","");
    output.remove(QString("%1>").arg(QCoreApplication::applicationDirPath().replace("/", "\\")));

    emit message(output);

    switch (m_operation)
    {
        case Distros:
            parserDistros(output);
            break;
        case USBPorts:
            parserUSBPorts(output);
            break;
        default:
            // m_operation = None;
            break;
    }

    if ((m_operations.count() > 0) &&
        ((m_operation == None) || (m_operation == Initializing)))
    {
        execOperation(m_operations.takeFirst());
    }
}

void
WSLInterface::readyReadStandardOutput()
{

}

void
WSLInterface::readyReadStandardError()
{

}
