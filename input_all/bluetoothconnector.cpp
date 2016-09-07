#include "bluetoothconnector.h"
#include <qdebug.h>

#include <QBluetoothUuid>

#include "core/metawearboard.h"

BluetoothConnector::BluetoothConnector(QObject *parent)
    : QObject(parent)
{
    // Create a discovery agent and connect to its signals
    m_discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(deviceDiscovered(const QBluetoothDeviceInfo&)));

    connect(m_discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));

    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
}

BluetoothConnector::~BluetoothConnector()
{
    qDeleteAll(m_devices);
    m_devices.clear();
}

QList<DeviceInfo*> BluetoothConnector::getDevices()
{
    return m_devices.values();
}

void BluetoothConnector::startDeviceDiscovery()
{
    m_discoveryAgent->start();
    qDebug() << "Scanning for devices...";
}

void BluetoothConnector::scanFinished()
{
    qDebug() << "Scanning finished.";
    if (m_devices.empty())
    {
        qDebug() << "No Low Energy devices found";
    }
    else
    {
        qDebug() << "Connecting to found devices.";
        QMap<QBluetoothUuid,DeviceInfo*>::iterator dev = m_devices.begin();
        for( ; dev != m_devices.end(); ++dev )
        {
            MetaWearDevice* mwDev = dynamic_cast<MetaWearDevice*>(dev.value());
            if(mwDev)
                mwDev->init();
        }
    }
}

// In your local slot, read information about the found devices
void BluetoothConnector::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qWarning() << "Discovered LE Device name: " << device.name() << " UUID: "
               << device.deviceUuid().toString();

    //check for unknown BTLE device
    if ( (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
         && ! m_devices.contains(device.deviceUuid()) )
    {
        qDebug() << "It is a new device. Add it to the list";

        MetaWearDevice* board = new MetaWearDevice(device, this);

        m_devices[device.deviceUuid()] = board;
        Q_EMIT updated();

        qDebug() << "Scanning for more...";
    }
}

void BluetoothConnector::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug() << "The Bluetooth adaptor is powered off, power it on before doing discovery.";
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qDebug() << "Writing or reading from the device resulted in an error.";
    else
        qDebug() << "An unknown error has occurred.";
}
