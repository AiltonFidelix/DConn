#ifndef WSLINTERFACE_H
#define WSLINTERFACE_H

#include <QObject>
#include <QProcess>

class WSLInterface : public QObject
{
    Q_OBJECT
public:
    enum Operation { None, Initializing, Distros, USBPorts };

    explicit WSLInterface(QObject *parent = nullptr);
    ~WSLInterface();

    void start();
    void nextOperation(Operation operation);

signals:
    void ready();
    void distros(QStringList distros);
    void usbPorts(QStringList ports);
    void message(QString message);

private slots:
    void started();
    void readyRead();
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    void execOperation(Operation operation);
    void parserDistros(QString input);
    void parserUSBPorts(QString input);

private:
    QProcess m_process;
    QString m_output;

    Operation m_operation;
    QList<Operation> m_operations;
};

#endif // WSLINTERFACE_H
