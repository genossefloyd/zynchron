#include "bluetoothconnector.h"
#include <qdebug.h>

#include <QBluetoothUuid>

BluetoothConnector::BluetoothConnector(QObject *parent)
    : QObject(parent)
    , m_currentDevice(QBluetoothDeviceInfo())
    , foundHeartRateService(false)
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
    return m_devices;
}

void BluetoothConnector::startDeviceDiscovery()
{
    qDeleteAll(m_devices);
    m_devices.clear();
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
        DeviceInfo *dev = new DeviceInfo(device);
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

void BluetoothConnector::connectToService(const QString &address)
{
    bool deviceFound = false;
    for (int i = 0; i < m_devices.size(); i++)
    {
        if (((DeviceInfo*)m_devices.at(i))->getAddress() == address )
        {
            m_currentDevice.setDevice(((DeviceInfo*)m_devices.at(i))->getDevice());
            qDebug() << "Connecting to device...";
            deviceFound = true;
            break;
        }
    }
    // we are running demo mode
    if (!deviceFound) {
        qDebug() << "No Device found...";
        return;
    }

    if (m_control)
    {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = 0;
    }

    m_control = new QLowEnergyController(m_currentDevice.getDevice(), this);
    connect(m_control, SIGNAL(connected()),
            this, SLOT(deviceConnected()));
    connect(m_control, SIGNAL(disconnected()),
            this, SLOT(deviceDisconnected()));
    connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)),
            this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(m_control, SIGNAL(discoveryFinished()),
            this, SLOT(serviceScanDone()));
    connect(m_control, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(controllerError(QLowEnergyController::Error)));

    m_control->connectToDevice();
}

void BluetoothConnector::deviceConnected()
{
    m_control->discoverServices();
}

void BluetoothConnector::deviceDisconnected()
{
    qDebug() << "Heart Rate service disconnected";
    qWarning() << "Remote device disconnected";
}

void BluetoothConnector::serviceDiscovered(const QBluetoothUuid &gatt)
{
    qDebug() << "Service discovered: " << gatt.toString() << ". Waiting for service scan to be done...";
    foundHeartRateService = gatt == QBluetoothUuid::HeartRate;
}

void BluetoothConnector::serviceScanDone()
{
    delete m_service;
    m_service = 0;

    if (foundHeartRateService) {
        qDebug() << "Connecting to service...";
        m_service = m_control->createServiceObject(
                    QBluetoothUuid(QBluetoothUuid::HeartRate), this);

        connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

        m_service->discoverDetails();
    }
}

void BluetoothConnector::controllerError(QLowEnergyController::Error error)
{
    qDebug() << "Cannot connect to remote device.";
    qWarning() << "Controller Error:" << error;
}

void BluetoothConnector::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s)
    {
        default:
            //nothing for now
            break;
    }
}
