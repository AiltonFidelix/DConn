#include "commanddetach.hpp"

#include <QDebug>
#include <QProcess>

CommandDetach::CommandDetach(CommandObserver *observer, const QString &busId)
    : _observer{observer},
      _busId{busId} {

    qInfo() << "CommandDetach::CommandDetach [BUS_ID]" << busId;
}

void CommandDetach::run() {

    constexpr int TIMEOUT_MS = 3'000;

    const QStringList args{
        QStringLiteral("detach"),
        QStringLiteral("--busid"),
        _busId,
    };

    QProcess process;

    process.start(QStringLiteral("usbipd"), args);

    const QString errorMessage = QStringLiteral("Failed to detach the device.");

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "CommandDetach::run Failed to execute 'usbipd detach' or operation timed out.";
        return notifyError(errorMessage);
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "CommandDetach::run Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "CommandDetach::run [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return notifyError(errorMessage);
    }

    if (_observer) {
        _observer->notifyDetached(_busId);
    }
}

void CommandDetach::notifyError(const QString &error) {

    qWarning() << "CommandDetach::notifyError [ERROR]" << error;

    if (_observer) {
        _observer->notifyError(error);
    }
}
