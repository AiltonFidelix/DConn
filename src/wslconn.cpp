#include "wslconn.h"
#include "./ui_wslconn.h"

WSLConn::WSLConn(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WSLConn)
{
    ui->setupUi(this);

    m_interface.start();

    connect(&m_interface, &WSLInterface::ready, this, &WSLConn::getDistros);
    connect(&m_interface, &WSLInterface::distros, this, &WSLConn::setDistros);
}

WSLConn::~WSLConn()
{
    delete ui;
}

void
WSLConn::getDistros()
{
    m_interface.getDistros();
}

void
WSLConn::setDistros(QStringList distros)
{
    ui->cbxDistros->addItems(distros);
}
