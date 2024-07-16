#ifndef OPDISTROS_H
#define OPDISTROS_H

#include "ioperation.h"

#include <QObject>

class OPDistros : public QObject, public iOperation
{
        Q_OBJECT
    public:
        explicit OPDistros(QObject *parent = nullptr);

        void runCommand();
        void setCommandArgs(QStringList args);
        void setProcess(QProcess *process);
        void parser(QString data);
        bool isComplete();

    signals:
        void distros(QJsonObject json);

    private:
        QProcess *m_process;
        bool m_isComplete;
};

#endif // OPDISTROS_H
