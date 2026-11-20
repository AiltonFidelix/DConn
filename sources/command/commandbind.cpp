#include "commandbind.hpp"

#include <QDebug>
#include <QProcess>

CommandBind::CommandBind(CommandObserver *observer, const QString &busId)
    : _observer{observer},
      _busId{busId} {

    qInfo() << "CommandBind::CommandBind [BUS_ID]" << busId;
}

void CommandBind::run() {

    constexpr int TIMEOUT_MS = 3'000;

    const QStringList args{
        QStringLiteral("bind"),
        QStringLiteral("--busid"),
        _busId,
    };

    QProcess process;

    process.start(QStringLiteral("usbipd"), args);

    const QString errorMessage = QStringLiteral("Failed to bind the device.\nVerify whether WSL is running or not.");

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "CommandBind::run Failed to execute 'usbipd bind' or operation timed out.";
        return notifyError(errorMessage);
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "CommandBind::run Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "CommandBind::run [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return notifyError(errorMessage);
    }

    if (_observer) {
        _observer->notifyBound(_busId);
    }
}

void CommandBind::notifyError(const QString &error) {

    qWarning() << "CommandBind::notifyError [ERROR]" << error;

    if (_observer) {
        _observer->notifyError(error);
    }
}
