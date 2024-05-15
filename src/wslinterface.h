#ifndef WSLINTERFACE_H
#define WSLINTERFACE_H

#include <QObject>
#include <QProcess>

class WSLInterface : public QObject
{
    Q_OBJECT
public:
    explicit WSLInterface(QObject *parent = nullptr);
    ~WSLInterface();

    void start();
    void getDistros();

signals:
    void ready();
    void distros(QStringList distros);

private slots:
    void started();
    void readyRead();
    void readyReadStandardOutput();
    void readyReadStandardError();

private:
    QProcess m_process;
};

#endif // WSLINTERFACE_H
