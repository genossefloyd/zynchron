#include "maingui.h"
#include "ui_maingui.h"

#include "deviceinfo.h"
#include <QBluetoothDeviceInfo>

MainGui::MainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainGui),
    m_connector(this)
{
    ui->setupUi(this);

    m_deviceListModel = new QStandardItemModel( 0, 1, this );
    ui->deviceList->setModel(m_deviceListModel);

    connect(&m_connector, SIGNAL(updated()), this, SLOT(updateDeviceList()));
}

MainGui::~MainGui()
{
    delete m_deviceListModel;
    delete ui;
}

void MainGui::updateDeviceList()
{
    m_deviceListModel->clear();

    QList<QBluetoothDeviceInfo*> devices = m_connector.getDevices();
    for( int i=0; i < devices.size(); i++)
    {
        m_deviceListModel->appendRow(new QStandardItem(devices[i]->name()));
    }
}

void MainGui::on_pushButton_clicked()
{
    m_connector.startDeviceDiscovery();
}

void MainGui::on_deviceList_doubleClicked(const QModelIndex &index)
{
    ui->deviceList->setDisabled(true);
    m_connector.connectToDevice(index.row());
}

void MainGui::on_button_LED_clicked()
{
    emit clicked_LED();
}
