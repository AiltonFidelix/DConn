#include "dconn.hpp"

#include <QMessageBox>
#include <QShowEvent>

#include "ui_dconn.h"

DConn::DConn(QWidget *parent)
    : QMainWindow{parent},
      _ui{new Ui::DConn},
      _service{} {

    _ui->setupUi(this);

    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    connect(_ui->actionAbout, &QAction::triggered, this, &DConn::onAbout);
    connect(_ui->cbDevices, &QComboBox::currentIndexChanged, this, &DConn::onSelectDevice);
    connect(_ui->btAction, &QPushButton::clicked, this, &DConn::onAction);

    connect(&_service, &CommandService::deviceAttached, this, &DConn::onAttached);
    connect(&_service, &CommandService::deviceDetached, this, &DConn::onDetached);
    connect(&_service, &CommandService::devicesReady, this, &DConn::onDevicesReady);
    connect(&_service, &CommandService::errorOccurred, this, &DConn::onErrorOccurred);
    connect(&_service, &CommandService::systemReady, this, &DConn::onShowDevices);
    connect(&_service, &CommandService::notReady, this, &DConn::onNotReady);

    disableActions();
}

DConn::~DConn() {
    delete _ui;
}

void DConn::start() {

    show();
    _service.verifySystem();
}

void DConn::onAbout() {

    QMessageBox::information(this, QStringLiteral("About DConn"),
                             QStringLiteral("<b>Author:</b> Ailton Fidelix<br>"
                                            "<b>Version:</b> %1<br>"
                                            "<b>Release date:</b> %2"
                                            "<br><p>DConn is an app for connecting USB devices to WSL.</p>")
                                 .arg(VERSION, DATE));
}

void DConn::onAction() {

    const auto device = _ui->cbDevices->currentData().value<USBIPDDeviceInfo>();

    disableActions();

    if (device.state == USBIPDStateEnum::Attached) {
        _service.detach(device);
    } else {
        _service.attach(device);
    }
}

void DConn::onAttached(const QString &busId) {

    auto currentDevice = _ui->cbDevices->currentData().value<USBIPDDeviceInfo>();

    if (currentDevice.busId == busId) {

        currentDevice.state = USBIPDStateEnum::Attached;

        _ui->cbDevices->setItemData(_ui->cbDevices->currentIndex(), QVariant::fromValue(currentDevice));

        _ui->lbState->setText(stateDescriptionFromState(currentDevice.state));
        _ui->btAction->setText(actionDescriptionFromState(currentDevice.state));
    }

    enableActions();
}

void DConn::onDetached(const QString &busId) {

    auto currentDevice = _ui->cbDevices->currentData().value<USBIPDDeviceInfo>();

    if (currentDevice.busId == busId) {

        currentDevice.state = USBIPDStateEnum::Shared;

        _ui->cbDevices->setItemData(_ui->cbDevices->currentIndex(), QVariant::fromValue(currentDevice));

        _ui->lbState->setText(stateDescriptionFromState(currentDevice.state));
        _ui->btAction->setText(actionDescriptionFromState(currentDevice.state));
    }

    enableActions();
}

void DConn::onDevicesReady(const USBIPDDeviceInfoList &devices) {

    _ui->cbDevices->clear();

    if (devices.isEmpty()) {
        return;
    }

    enableActions();

    for (const USBIPDDeviceInfo &device : devices) {
        _ui->cbDevices->addItem(device.device, QVariant::fromValue(device));
    }
}

void DConn::onSelectDevice() {

    const auto currentDevice = _ui->cbDevices->currentData().value<USBIPDDeviceInfo>();

    _ui->lbState->setText(stateDescriptionFromState(currentDevice.state));
    _ui->btAction->setText(actionDescriptionFromState(currentDevice.state));
}

void DConn::onErrorOccurred(const QString &error) {

    QMessageBox::warning(this, QStringLiteral("Error"), error);

    enableActions();
}

void DConn::onShowDevices() {
    _service.showDevices();
}

void DConn::onNotReady() {

    QMessageBox::warning(this, QStringLiteral("Error"), QStringLiteral("<p>usbipd version >= %0 is required.</p>").arg(USBIPD_VERSION));

    exit(0);
}

void DConn::enableActions() {
    _ui->cbDevices->setEnabled(true);
    _ui->btAction->setEnabled(true);
}

void DConn::disableActions() {
    _ui->cbDevices->setEnabled(false);
    _ui->btAction->setEnabled(false);
}

QString DConn::stateDescriptionFromState(const USBIPDStateEnum state) {

    switch (state) {
        case USBIPDStateEnum::Attached:
            return QStringLiteral("Attached");
        case USBIPDStateEnum::NotShared:
            return QStringLiteral("NotShared");
        case USBIPDStateEnum::Shared:
            return QStringLiteral("Shared");
        default:
            return QStringLiteral("Unknown");
    }
}

QString DConn::actionDescriptionFromState(const USBIPDStateEnum state) {

    switch (state) {
        case USBIPDStateEnum::Attached:
            return QStringLiteral("Detach");
        case USBIPDStateEnum::NotShared:
        case USBIPDStateEnum::Shared:
        default:
            return QStringLiteral("Attach");
    }
}
