#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

#include "deviceinfo.h"
#include "metawearboard.h"

class BluetoothConnector : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothConnector(QObject *parent = 0);
    ~BluetoothConnector();

    void startDeviceDiscovery();
    void connectToDevice(const QString &address);
    void connectToDevice(int index);

    QList<QBluetoothDeviceInfo*> getDevices();

private:
    void connectToDevice();

    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QList<QBluetoothDeviceInfo*> m_devices;
    QBluetoothDeviceInfo* m_currentDevice;

    metaweardevice* m_board;

signals:
    void updated();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();
};

#endif // BLUETOOTHCONNECTOR_H
