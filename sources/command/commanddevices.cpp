#include "commanddevices.hpp"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>

#include "usbipddeviceinfo.hpp"
#include "usbipdstatehelper.hpp"

CommandDevices::CommandDevices(CommandObserver *observer)
    : _observer{observer} {

    qInfo() << "CommandDevices::CommandDevices";
}

void CommandDevices::run() {

    constexpr int TIMEOUT_MS = 3'000;

    const QStringList args{
        QStringLiteral("list"),
    };

    QProcess process;

    process.start(QStringLiteral("usbipd"), args);

    const QString errorMessage = QStringLiteral("Failed to list the devices.");

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "CommandDevices::run Failed to execute 'usbipd list' or operation timed out.";
        return notifyError(errorMessage);
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "CommandDevices::run Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "CommandDevices::run [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return notifyError(errorMessage);
    }

    const QByteArray output = process.readAllStandardOutput();

    QString outputString = QString::fromLocal8Bit(output);

    QTextStream stream(&outputString);
    USBIPDDeviceInfoList devices{};

    bool isFirstLine = true;

    while (!stream.atEnd()) {
        const QString line = stream.readLine().trimmed();

        if (line.isEmpty()) {
            continue;
        }

        if (isFirstLine) {

            isFirstLine = false;

            if (line.startsWith(QStringLiteral("BUSID"), Qt::CaseInsensitive)) {
                continue;
            }
        }

        static QRegularExpression rx(QStringLiteral(R"(^(\S+)\s+([0-9a-fA-F]{4}:[0-9a-fA-F]{4})\s+(.*?)\s{2,}([A-Za-z\s]+)$)"));
        QRegularExpressionMatch match = rx.match(line);

        if (!match.hasMatch()) {
            continue;
        }

        USBIPDDeviceInfo device{};

        device.busId = match.captured(1);
        device.device = match.captured(3).trimmed();
        device.state = USBIPDStateHelper::stringToState(match.captured(4).trimmed());

        devices.append(device);
    }

    if (_observer) {
        _observer->notifyDevices(devices);
    }
}

void CommandDevices::notifyError(const QString &error) {

    qWarning() << "CommandDevices::notifyError [ERROR]" << error;

    if (_observer) {
        _observer->notifyError(error);
    }
}
