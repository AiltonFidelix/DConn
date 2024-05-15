#include "wslinterface.h"

#include <QDebug>

WSLInterface::WSLInterface(QObject *parent)
    : QObject(parent)
{
    connect(&m_process, &QProcess::started, this, &WSLInterface::started);
    connect(&m_process, &QProcess::readyRead, this, &WSLInterface::readyRead);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &WSLInterface::readyReadStandardOutput);
    connect(&m_process, &QProcess::readyReadStandardError, this, &WSLInterface::readyReadStandardError);
}

WSLInterface::~WSLInterface()
{
    m_process.close();
}

void WSLInterface::start()
{
    m_process.start("cmd", QStringList());
}

void
WSLInterface::getDistros()
{
    m_process.write("wsl --list\r\n");
}

void
WSLInterface::started()
{
    m_process.write("wsl --list\r\n");
}

void
WSLInterface::readyRead()
{
    QString output = QString::fromLatin1(m_process.readAll());
    output.replace('\0',R"(\u0000)");
    output.replace("\\u0000","");
    qDebug() << output.trimmed().split("\r\n");

    emit distros(output.trimmed().split("\r\n"));
}

void
WSLInterface::readyReadStandardOutput()
{
    // qDebug() << m_process.readAllStandardOutput();
}

void
WSLInterface::readyReadStandardError()
{

}
