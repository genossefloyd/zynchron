#include "bluetoothconnector.h"
#include <qdebug.h>

#include <QBluetoothUuid>

#include "core/metawearboard.h"

BluetoothConnector::BluetoothConnector(QObject *parent)
    : QObject(parent)
    , m_currentDevice(NULL)
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
    m_currentDevice = NULL;
}

QList<QBluetoothDeviceInfo*> BluetoothConnector::getDevices()
{
    return m_devices;
}

void BluetoothConnector::startDeviceDiscovery()
{
    qDeleteAll(m_devices);
    m_devices.clear();
    m_currentDevice = NULL;

    m_discoveryAgent->start();
    qDebug() << "Scanning for devices...";
}

void BluetoothConnector::scanFinished()
{
    if (m_devices.size() == 0)
        qDebug() << "No Low Energy devices found";
}

// In your local slot, read information about the found devices
void BluetoothConnector::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        qWarning() << "Discovered LE Device name: " << device.name() << " Address: "
                   << device.address().toString();

        QBluetoothDeviceInfo* dev = new QBluetoothDeviceInfo(device);
        m_devices.append(dev);
        qDebug() << "Low Energy device found. Scanning for more...";

        Q_EMIT updated();
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

void BluetoothConnector::connectToDevice(const QString &address)
{
    bool deviceFound = false;
    for (int index = 0; index < m_devices.size(); index++)
    {
        if (m_devices.at(index)->address().toString() == address )
        {
            m_currentDevice = m_devices.at(index);
            qDebug() << "Connecting to device...";
            deviceFound = true;
            break;
        }
    }
    // we are running demo mode
    if (deviceFound)
    {
        qDebug() << "No Device found...";
        return;
    }
    connectToDevice();
}

void BluetoothConnector::connectToDevice(int index)
{
    if( index < 0 || index >= m_devices.size() )
    {
        qDebug() << "No Device with index " << index << "available.";
        return;
    }
    m_currentDevice = m_devices.at(index);

    connectToDevice();
}

void BluetoothConnector::connectToDevice()
{
    qDebug() << "Connecting to " << m_currentDevice->name();
    m_board = new metaweardevice(this);
    m_board->init(new QLowEnergyController(*m_currentDevice, this));
}
