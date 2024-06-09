#ifndef WSLHANDLE_H
#define WSLHANDLE_H

#include <QObject>
#include <QProcess>
#include <QSharedPointer>

#include "ioperation.h"
#include "opfactory.h"

class WSLHandle : public QObject
{
    Q_OBJECT
public:
    explicit WSLHandle(QObject *parent = nullptr);
    ~WSLHandle();

    void start(QProcess *process);
    void nextOperation(iOperation *operation);

signals:
    void ready();
    void distros(QStringList distros);
    void usbPorts(QHash<QString, QStringList> ports);
    void message(QString message);

private slots:
    void started();
    void readyRead();
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    void execOperation(iOperation *operation);
    void parserDistros(QString input);
    void parserUSBPorts(QString input);

private:
    bool m_hasInit;
    QProcess *m_process;
    QString m_output;

    // QList<Operation> m_operations;

    QSharedPointer<iOperation> m_operation;
    QList<iOperation*> m_operations;
};

#endif // WSLHANDLE_H
