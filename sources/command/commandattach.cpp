#include "commandattach.hpp"

#include <QDebug>
#include <QProcess>

CommandAttach::CommandAttach(CommandObserver *observer, const QString &busId)
    : _observer{observer},
      _busId{busId} {

    qInfo() << "CommandAttach::CommandAttach [BUS_ID]" << busId;
}

void CommandAttach::run() {

    constexpr int TIMEOUT_MS = 3'000;

    const QStringList args{
        QStringLiteral("attach"),
        QStringLiteral("--wsl"),
        QStringLiteral("--busid"),
        _busId,
    };

    QProcess process;

    process.start(QStringLiteral("usbipd"), args);

    const QString errorMessage = QStringLiteral("Failed to attach the device.\nVerify whether WSL is running or not.");

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "CommandAttach::run Failed to execute 'usbipd attach' or operation timed out.";
        return notifyError(errorMessage);
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "CommandAttach::run Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "CommandAttach::run [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return notifyError(errorMessage);
    }

    if (!verifyState(_busId, USBIPDStateEnum::Attached)) {
        return notifyError(errorMessage);
    }

    if (_observer) {
        _observer->notifyAttached(_busId);
    }
}

void CommandAttach::notifyError(const QString &error) {

    qWarning() << "CommandAttach::notifyError [ERROR]" << error;

    if (_observer) {
        _observer->notifyError(error);
    }
}
