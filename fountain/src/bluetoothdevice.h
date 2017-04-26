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
        Connecting,
        Connected,
		Pairing,
		Paired
    };

    BluetoothDevice(const std::string& name, const std::string& addr);
    explicit BluetoothDevice(BluetoothDevice& device);
    virtual ~BluetoothDevice();

    std::string getName() const ;
    std::string getAddress() const;

    State getState() const { return m_state; }
    void connect();

protected:
    static void createGattConnection(BluetoothDevice* device);

    std::string m_deviceName;
    std::string m_deviceAddress;
    gatt_connection_t* m_connection;
    State m_state;

    std::thread m_thread;
    static std::mutex sm_mutexConnect;
};

#endif // BLUETOOTH_DEVICE_H
