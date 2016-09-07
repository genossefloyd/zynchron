#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QObject>
#include <QDebug>
#include <QString>

#include "deviceinfo.h"
#include "metaweardevice.h"

class BluetoothConnector : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothConnector(QObject *parent = 0);
    ~BluetoothConnector();

    void startDeviceDiscovery();

    QList<DeviceInfo*> getDevices();

private:
    void connectToDevice();

    QBluetoothDeviceDiscoveryAgent* m_discoveryAgent;
    QMap<QBluetoothUuid,DeviceInfo*> m_devices;

signals:
    void updated();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
};

#endif // BLUETOOTHCONNECTOR_H
