#include "wslconn.h"
#include "./ui_wslconn.h"

#include "opdistros.h"

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
    // connect(&m_handle, &WSLHandle::distros, this, &WSLConn::setDistros);
    // connect(&m_handle, &WSLHandle::usbPorts, this, &WSLConn::setUSBPorts);

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
    QJsonValue value = json.value("distros");
    QJsonArray array = value.toArray();

    qDebug() << value;

    for (QVariant distro : array.toVariantList())
    {
        ui->cbxDistros->addItem(distro.toString());
    }
}

void
WSLConn::setUSBPorts(QHash<QString, QStringList> ports)
{
    QHashIterator<QString, QStringList> it(ports);

    while (it.hasNext())
    {
        it.next();
        ui->cbxUsbPorts->addItem(it.key(), it.value());
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
    ui->lblShared->setText(info.at(1));
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

    // m_handle.nextOperation(m_factory.create(OPFactory::USBPorts));
}
