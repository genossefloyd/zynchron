#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include "bluetoothdevice.h"

#include <mutex>
#include <thread>
#include <list>

class BluetoothConnector
{
public:
	typedef std::list<BluetoothDevice*> DeviceList;

public:
    explicit BluetoothConnector();
    ~BluetoothConnector();

    bool startDeviceDiscovery();
    DeviceList getDevices();

private:
    static DeviceList startScan(int device_desc, int timeout);
    static int enableScan(int device_desc);
    static int disableScan(int device_desc);

    int m_deviceDescriptor;

    std::mutex m_mutexDiscover;
	std::list<BluetoothDevice*> m_devices;
};

#endif // BLUETOOTHCONNECTOR_H
