#include "maingui.h"
#include "ui_maingui.h"

#include "deviceinfo.h"

MainGui::MainGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainGui)
{
    ui->setupUi(this);

    m_deviceListModel = new QStandardItemModel( 0, 1, this );
    ui->deviceList->setModel(m_deviceListModel);

    connect(&connector, SIGNAL(updated()), this, SLOT(updateDeviceList()));
}

MainGui::~MainGui()
{
    delete m_deviceListModel;
    delete ui;
}

void MainGui::updateDeviceList()
{
    m_deviceListModel->clear();

    QList<DeviceInfo*> devices = connector.getDevices();
    for( int i=0; i < devices.size(); i++)
    {
        m_deviceListModel->appendRow(new QStandardItem(devices[i]->getAddress()));
    }
}

void MainGui::on_pushButton_clicked()
{
    connector.startDeviceDiscovery();
}

void MainGui::on_deviceList_doubleClicked(const QModelIndex &index)
{
    connector.connectToService(index.data().toString());
}
