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

    emit message(output.trimmed());

    bool opIsComplete = true;

    if (m_operation.data() != nullptr)
    {
        m_operation->parser(output);
        opIsComplete = m_operation->isComplete();
    }

    if (m_operations.count() > 0 && opIsComplete)
    {
        iOperation *operation = m_operations.takeFirst();
        m_operation.reset(operation);
        m_operation->runCommand();
    }
}
