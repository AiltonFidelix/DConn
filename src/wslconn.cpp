#include "wslconn.h"
#include "./ui_wslconn.h"

#include <QMessageBox>

WSLConn::WSLConn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WSLConn)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    connect(ui->actionAbout, &QAction::triggered, this, &WSLConn::about);
    connect(&m_interface, &WSLInterface::message, ui->pteLogs, &QPlainTextEdit::appendPlainText);
    connect(&m_interface, &WSLInterface::distros, this, &WSLConn::setDistros);
    connect(&m_interface, &WSLInterface::usbPorts, this, &WSLConn::setUSBPorts);

    ui->pteLogs->appendPlainText("Initializing logs...\n");
    m_interface.start();
}

WSLConn::~WSLConn()
{
    delete ui;
}

void
WSLConn::setDistros(QStringList distros)
{
    ui->cbxDistros->addItems(distros);
}

void
WSLConn::setUSBPorts(QStringList ports)
{
    ui->cbxUsbPorts->addItems(ports);
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
WSLConn::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    m_interface.nextOperation(WSLInterface::Operation::Distros);
    m_interface.nextOperation(WSLInterface::Operation::USBPorts);
}
