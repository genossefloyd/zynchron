#include "mqttoutput.h"
#include "bluetoothconnector.h"
#include "metaweardevice.h"

#include <iostream>
int main()
{
	std::cout << "Data Fountain active" << std::endl;
	//OutputHandler* out = new OutputHandler();
	MqttOutput* out = MqttOutput::instance();
	//out->init("127.0.0.1",1883);
	//out->init("192.168.178.20",1883);
	out->init("broker.mqttdashboard.com",1883);
	//out->toogleDummy();

#if 1
	BluetoothDevice device("MetaWearDirect", "C7:5E:96:DA:F4:8E");
	device.connect();
	while(device.getState() == BluetoothDevice::Connecting) base::Thread::sleep(1);
	if(device.getState() == BluetoothDevice::Connected)
	{
		MetaWearDevice* mwDev = new MetaWearDevice(device, out);
		mwDev->initialize();
		while(mwDev->getState() != BluetoothDevice::Disconnected) {
			base::Thread::sleep(1);
			if(mwDev->getState() == BluetoothDevice::Paired) break;
		}

		if(mwDev->getState() == BluetoothDevice::Paired) {
			mwDev->fetchData();
			while(mwDev->getState() != BluetoothDevice::Disconnected) base::Thread::sleep(1);
		}
	}
#else
	BluetoothConnector connector;
	if(!connector.startDeviceDiscovery())
	{
		return 1;
	}

	std::list<MetaWearDevice*> metawearDevices;
	BluetoothConnector::DeviceList devices = connector.getDevices();
	BluetoothConnector::DeviceList::iterator itDevice = devices.begin();
	while(itDevice != devices.end())
	{
		BluetoothDevice* blDev = *itDevice;
		if(blDev->getState() == BluetoothDevice::Connected)
		{
			MetaWearDevice* mwDev = new MetaWearDevice(*blDev, out);
			mwDev->initialize();
			metawearDevices.push_back(mwDev);
		}
	}
	while(true) base::Thread::sleep(1);
#endif
	return 0;
}
