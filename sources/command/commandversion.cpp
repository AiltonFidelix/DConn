#include "commandversion.hpp"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>

CommandVersion::CommandVersion(CommandObserver *observer)
    : _observer{observer} {

    qInfo() << "CommandVersion::CommandVersion";
}

void CommandVersion::run() {

    constexpr int TIMEOUT_MS = 3'000;

    const QStringList args{
        QStringLiteral("--version"),
    };

    QProcess process;

    process.start(QStringLiteral("usbipd"), args);

    const QString errorMessage = QStringLiteral("Failed to verify usbipd --version.");

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "CommandVersion::run Failed to execute 'usbipd --version' or operation timed out.";
        return notifyError(errorMessage);
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "CommandVersion::run Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "CommandVersion::run [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return notifyError(errorMessage);
    }

    const QByteArray output = process.readAllStandardOutput();

    if (_observer) {

        const QVersionNumber minVersion = QVersionNumber::fromString(QString::fromLatin1(MIN_USBIPD_VERSION));
        const QVersionNumber detectedVersion = parseVersion(output);

        if (!minVersion.isNull() && !detectedVersion.isNull() && (detectedVersion >= minVersion)) {
            _observer->notifySystemReady();
        } else {
            _observer->notifyNotReady();
        }
    }
}

QVersionNumber CommandVersion::parseVersion(const QByteArray &output) {

    const QString text = QString::fromUtf8(output).trimmed();

    static const QRegularExpression re(QStringLiteral(R"((\d+(?:\.\d+){1,3}))"));

    const QRegularExpressionMatch match = re.match(text);

    if (!match.hasMatch()) {
        return {};
    }

    return QVersionNumber::fromString(match.captured(1));
}

void CommandVersion::notifyError(const QString &error) {

    qWarning() << "CommandVersion::notifyError [ERROR]" << error;

    if (_observer) {
        _observer->notifyError(error);
    }
}
