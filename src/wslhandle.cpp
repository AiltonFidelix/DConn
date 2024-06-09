#include "wslhandle.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QDebug>

WSLHandle::WSLHandle(QObject *parent)
    : QObject(parent)
{
    m_hasInit = false;
}

WSLHandle::~WSLHandle()
{
    m_process->close();
    qDeleteAll(m_operations);
}

void
WSLHandle::start(QProcess *process)
{
    m_process = process;

    connect(m_process, &QProcess::started, this, &WSLHandle::started);
    connect(m_process, &QProcess::readyRead, this, &WSLHandle::readyRead);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &WSLHandle::readyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &WSLHandle::readyReadStandardError);

    m_process->start("cmd", QStringList());
}

void
WSLHandle::nextOperation(iOperation *operation)
{
    if (m_operations.count() == 0 && m_hasInit)
    {
        m_operation.reset(operation);
        m_operation->runCommand();
    }
    else
    {
        m_operations.append(operation);
    }
}

void
WSLHandle::execOperation(iOperation *operation)
{
    if (!m_process->isWritable())
    {
        return;
    }

    // m_operation = operation;

    // switch (operation)
    // {
    //     case Distros:
    //         m_process.write("wsl --list\r\n");
    //         break;
    //     case USBPorts:
    //         m_process.write("usbipd list\r\n");
    //         break;
    //     case Shared:
    //     {
    //         QString cmd = QString("usbipd attach --wsl %1 --busid %2\r\n").arg(m_args.at(0), m_args.at(1));
    //         m_process.write(cmd.toUtf8());
    //     }
    //         break;
    //     default:
    //         break;
    // }
}

void
WSLHandle::parserDistros(QString input)
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

        // m_operation = None;
    }
}

void
WSLHandle::parserUSBPorts(QString input)
{
    QStringList lines = input.trimmed().split("\r\n");
    QHash<QString, QStringList> usbportsList;

    static QRegularExpression regex("\\s{2,}");

    for (int i = 2; i < lines.count(); ++i)
    {
        QStringList info = lines.at(i).trimmed().split(regex);

        if (info.count() >= 3)
        {
            QStringList data;
            data << info.at(0) << info.at(3);
            usbportsList[info.at(2)] = data;
        }
    }

    if (usbportsList.count() > 0)
    {
        emit usbPorts(usbportsList);

        // m_operation = None;
    }
}

void
WSLHandle::started()
{
    emit ready();
}

void
WSLHandle::readyRead()
{
    m_hasInit = true;
    QString output = QString::fromLatin1(m_process->readAll());
    output.replace('\0',R"(\u0000)");
    output.replace("\\u0000","");
    output.remove(QString("%1>").arg(QCoreApplication::applicationDirPath().replace("/", "\\")));

    emit message(output);

    if (m_operation.data() != nullptr)
    {
        m_operation->parser(output);
    }

    // switch (m_operation)
    // {
    //     case Distros:
    //         parserDistros(output);
    //         break;
    //     case USBPorts:
    //         parserUSBPorts(output);
    //         break;
    //     default:
    //         // m_operation = None;
    //         break;
    // }

    if (m_operations.count() > 0)
    {
        iOperation *operation = m_operations.takeFirst();
        m_operation.reset(operation);
        m_operation->runCommand();
    }
}

void
WSLHandle::readyReadStandardOutput()
{

}

void
WSLHandle::readyReadStandardError()
{

}
