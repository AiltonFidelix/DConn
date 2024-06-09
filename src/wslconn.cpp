#include "wslconn.h"
#include "./ui_wslconn.h"

#include "opdistros.h"
#include "opusbports.h"

#include <QMessageBox>
#include <QJsonArray>

WSLConn::WSLConn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WSLConn)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

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
    qDebug() << json;
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
    qDebug() << json;
    QJsonValue value = json.value("ports");
    QJsonArray array = value.toArray();

    for (const QVariant &port : array.toVariantList())
    {
        QStringList info = port.toStringList();
        ui->cbxUsbPorts->addItem(info.first(), info);
    }
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
WSLConn::updateStatus()
{
    QStringList info = ui->cbxUsbPorts->currentData().toStringList();
    ui->lblShared->setText(info.at(2));
}

void
WSLConn::attachPort()
{
    QStringList portInfo = ui->cbxUsbPorts->currentData().toStringList();
    QStringList args = {ui->cbxDistros->currentText(), portInfo.at(0)};
    m_handle.nextOperation(m_factory.create(OPFactory::Attach, args));
}

void
WSLConn::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    OPDistros *distros = static_cast<OPDistros*>(m_factory.create(OPFactory::Distros));
    connect(distros, &OPDistros::distros, this, &WSLConn::setDistros);
    m_handle.nextOperation(distros);

    OPUSBPorts *usbPorts = static_cast<OPUSBPorts*>(m_factory.create(OPFactory::USBPorts));
    connect(usbPorts, &OPUSBPorts::usbPorts, this, &WSLConn::setUSBPorts);
    m_handle.nextOperation(usbPorts);
}
