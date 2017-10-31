#ifndef BLUETOOTH_DEVICE_H
#define BLUETOOTH_DEVICE_H

#include "gattlib.h"

#include <mutex>
#include <thread>
#include <string>
#include <vector>
#include <map>

class BluetoothDevice
{
public:
    enum State
    {
    	Disconnected,
		Connecting, //establishing bluetooth connection
		Connected, //bluetooth connection established
		Pairing, //establishing connection in custom top-layer protocol
		Paired //custom connection established
    };

    //ctor to create a new bluetooth device with given name and mac address
    BluetoothDevice(const std::string& name, const std::string& addr);

    //'moving' constructor - transfers connection credentials from given 'device' to newly created one
    explicit BluetoothDevice(BluetoothDevice& device);

    //dtor
    virtual ~BluetoothDevice();

    std::string getName() const ; //get name/device identifier of this bluetooth device
    std::string getAddress() const; //get mac address of this bluetooth device

    //return connection state
    State getState() const { return m_state; }

    bool connect(); //try to establish bluetooth connection
    void disconnect(); //shutdown bluetooth connection

protected:
    static void discoverChars(gatt_connection_t* gatt_connection);

    std::string m_deviceName;
    std::string m_deviceAddress;
    gatt_connection_t* m_connection;
    State m_state;

    std::thread m_thread;
    static std::mutex sm_mutexConnect;
};

#endif // BLUETOOTH_DEVICE_H
