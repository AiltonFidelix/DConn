#ifndef WSLHANDLE_H
#define WSLHANDLE_H

#include <QObject>
#include <QProcess>
#include <QSharedPointer>

#include "ioperation.h"

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
    void message(QString message);

private slots:
    void started();
    void readyRead();

private:
    bool m_hasInit;
    QProcess *m_process;

    QSharedPointer<iOperation> m_operation;
    QList<iOperation*> m_operations;
};

#endif // WSLHANDLE_H
