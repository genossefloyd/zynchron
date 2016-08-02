#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>

#include "core/metawearboard.h"

class BluetoothConnector : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothConnector(QObject *parent = 0);
    ~BluetoothConnector();

    void startDeviceDiscovery();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
};

#endif // BLUETOOTHCONNECTOR_H
