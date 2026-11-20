#include "usbipd.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

bool USBIPD::verifyState(const QString &busId, const USBIPDStateEnum state) const {

    constexpr int TIMEOUT_MS = 3'000;

    QProcess process;

    process.start(QStringLiteral("usbipd"), QStringList() << QStringLiteral("state"));

    if (!process.waitForFinished(TIMEOUT_MS)) {
        qWarning() << "USBIPD::verifyState Failed to execute or operation timed out.";
        return false;
    }

    const QProcess::ExitStatus exitStatus = process.exitStatus();
    const int exitCode = process.exitCode();

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {

        qWarning() << "USBIPD::verifyState Failed to execute properly."
                   << "[EXIT_STATUS]" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crash")
                   << "[EXIT_CODE]" << exitCode;

        const QByteArray errorData = process.readAllStandardError();

        if (!errorData.isEmpty()) {
            qWarning() << "USBIPD::verifyState [ERROR_DATA]" << QString::fromUtf8(errorData);
        }

        return false;
    }

    const QByteArray jsonData = process.readAllStandardOutput();

    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "USBIPD::verifyState [PARSE_ERROR]" << parseError.errorString();
        return false;
    }

    if (!jsonDoc.isObject()) {
        qWarning() << "USBIPD::verifyState Root JSON structure is not an object.";
        return false;
    }

    const QJsonObject rootObj = jsonDoc.object();
    const QJsonValue devicesValue = rootObj.value(QStringLiteral("Devices"));

    if (!devicesValue.isArray()) {
        qWarning() << "USBIPD::verifyState JSON does not contain a valid 'Devices' array.";
        return false;
    }

    const QJsonArray devicesArray = devicesValue.toArray();

    for (const QJsonValue &value : devicesArray) {

        if (!value.isObject()) {
            continue;
        }

        const QJsonObject deviceObj = value.toObject();
        const QString currentBusId = deviceObj.value(QStringLiteral("BusId")).toString();

        if (busId != currentBusId) {
            continue;
        }

        switch (state) {
            case USBIPDStateEnum::Attached:
                return !deviceObj.value(QStringLiteral("StubInstanceId")).isNull();
            case USBIPDStateEnum::Shared:
                return !deviceObj.value(QStringLiteral("PersistedGuid")).isNull();
            default:
                return false;
        }
    }

    return false;
}
