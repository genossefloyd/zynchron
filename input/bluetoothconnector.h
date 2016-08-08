#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>

#include "core/metawearboard.h"

#include "deviceinfo.h"

class BluetoothConnector : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothConnector(QObject *parent = 0);
    ~BluetoothConnector();

    void startDeviceDiscovery();
    void connectToService(const QString &address);

    QList<DeviceInfo*> getDevices();

private:
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QList<DeviceInfo*> m_devices;
    DeviceInfo m_currentDevice;

    QLowEnergyController *m_control;
    QLowEnergyService *m_service;

    bool foundHeartRateService;

Q_SIGNALS:
    void updated();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinished();

    void deviceConnected();
    void deviceDisconnected();
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error);

    void serviceStateChanged(QLowEnergyService::ServiceState s);
};

#endif // BLUETOOTHCONNECTOR_H
