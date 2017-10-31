#ifndef BLUETOOTHCONNECTOR_H
#define BLUETOOTHCONNECTOR_H

#include "bluetoothdevice.h"

#include <mutex>
#include <thread>
#include <list>

//class to scan and find available bluetooth devices
class BluetoothConnector
{
public:
	typedef std::list<BluetoothDevice*> DeviceList;

public:
    explicit BluetoothConnector();
    ~BluetoothConnector();

    //start a new scan for available bluetooth devices
    //returns true if scan was finished without error and if at least one device was founds
    bool startDeviceDiscovery();

    //return a list of all found bluetooth devices
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
