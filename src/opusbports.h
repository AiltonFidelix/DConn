#ifndef OPUSBPorts_H
#define OPUSBPorts_H

#include "ioperation.h"

#include <QObject>

class OPUSBPorts : public QObject, public iOperation
{
        Q_OBJECT
    public:
        explicit OPUSBPorts(QObject *parent = nullptr);

        void runCommand();
        void setCommandArgs(QStringList args);
        void setProcess(QProcess *process);
        void parser(QString data);
        bool isComplete();

    signals:
        void usbPorts(QJsonObject json);

    private:
        QProcess *m_process;
        bool m_isComplete;
};

#endif // OPUSBPorts_H
