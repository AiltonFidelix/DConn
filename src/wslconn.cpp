#include "wslconn.h"
#include "ui_wslconn.h"
#include "settings.h"

#include "opattach.h"
#include "opdetach.h"
#include "opdistros.h"
#include "opusbports.h"

#include <QMessageBox>
#include <QJsonArray>
#include <QTimer>

WSLConn::WSLConn(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::WSLConn),
    m_currentItem(0),
    m_detach(false)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    connect(ui->actionSettings, &QAction::triggered, this, &WSLConn::settings);
    connect(ui->actionAbout, &QAction::triggered, this, &WSLConn::about);
    connect(ui->cbxUsbPorts, &QComboBox::currentIndexChanged, this, &WSLConn::updateStatus);
    connect(ui->btnAttach, &QPushButton::clicked, this, &WSLConn::attachPort);
    connect(&m_handle, &WSLHandle::message, ui->pteLogs, &QPlainTextEdit::appendPlainText);

    ui->pteLogs->appendPlainText("Initializing logs...\n");

    QProcess *process = new QProcess(this);

    m_handle.start(process);
    m_factory.setProcess(process);
}

WSLConn::~WSLConn()
{
    delete ui;
}

void
WSLConn::setDistros(QJsonObject json)
{
    ui->cbxDistros->clear();

    QJsonValue value = json.value("distros");
    QJsonArray array = value.toArray();

    qDebug() << value;

    for (const QVariant &distro : array.toVariantList())
    {
        ui->cbxDistros->addItem(distro.toString());
    }
}

void
WSLConn::setUSBPorts(QJsonObject json)
{
    ui->cbxUsbPorts->clear();

    qDebug() << json;
    QJsonValue value = json.value("ports");
    QJsonArray array = value.toArray();

    for (const QVariant &port : array.toVariantList())
    {
        QStringList info = port.toStringList();
        ui->cbxUsbPorts->addItem(info.first(), info);
    }

    ui->cbxUsbPorts->setCurrentIndex(m_currentItem);
}

void
WSLConn::about()
{
    QMessageBox::information(this, "About",
                             QString("<b>Author:</b> Ailton Fidelix<br>"
                                     "<b>Version:</b> %1<br>"
                                     "<b>Date:</b> %2").arg(VERSION, DATE));
}

void
WSLConn::settings()
{
    Settings settings(this);
    settings.exec();
}

void
WSLConn::showPortsInfo()
{
    auto updatePorts = [this] () -> void
    {
        OPUSBPorts *usbPorts = static_cast<OPUSBPorts*>(m_factory.create(OPFactory::USBPorts));
        connect(usbPorts, &OPUSBPorts::usbPorts, this, &WSLConn::setUSBPorts);
        m_handle.nextOperation(usbPorts);
    };

    QTimer::singleShot(1000, this, updatePorts);
}

void
WSLConn::updateStatus()
{
    QStringList info = ui->cbxUsbPorts->currentData().toStringList();

    if (info.count() == 3)
    {
        m_detach = (info.at(2) == "Attached");
        ui->lblShared->setText(info.at(2));
        ui->btnAttach->setText(m_detach ? "Detach" : "Attach");
    }
}

void
WSLConn::attachPort()
{
    QStringList portInfo = ui->cbxUsbPorts->currentData().toStringList();
    QStringList args = {ui->cbxDistros->currentText(), portInfo.at(1)};

    m_currentItem = ui->cbxUsbPorts->currentIndex();

    if (m_detach)
    {
        OPDetach *detach = static_cast<OPDetach*>(m_factory.create(OPFactory::Detach, args));
        connect(detach, &OPDetach::detached, this, &WSLConn::showPortsInfo);

        m_handle.nextOperation(detach);
    }
    else
    {
        OPAttach *attach = static_cast<OPAttach*>(m_factory.create(OPFactory::Attach, args));
        connect(attach, &OPAttach::attached, this, &WSLConn::showPortsInfo);

        m_handle.nextOperation(attach);
    }
}

void
WSLConn::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    OPDistros *distros = static_cast<OPDistros*>(m_factory.create(OPFactory::Distros));
    connect(distros, &OPDistros::distros, this, &WSLConn::setDistros);
    m_handle.nextOperation(distros);

    showPortsInfo();
}
