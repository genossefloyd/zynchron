#include "bluetoothconnector.h"
#include <qdebug.h>

#include <QBluetoothUuid>

BluetoothConnector::BluetoothConnector(QObject *parent)
    : QObject(parent)
{

}

BluetoothConnector::~BluetoothConnector(){}

void BluetoothConnector::startDeviceDiscovery()
{
    qDebug() << "Searching for Devices!";

    // Create a discovery agent and connect to its signals
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    // Start a discovery
    discoveryAgent->start();

    qDebug() << "Stopped searching.";
}

// In your local slot, read information about the found devices
void BluetoothConnector::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.deviceUuid().toString() << ')';
}
