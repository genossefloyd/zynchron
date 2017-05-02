#include "mqttoutput.h"
#include "bluetoothconnector.h"
#include "metaweardevice.h"

#include <iostream>
int main(int argc, char *argv[])
{
	std::cout << "Data Fountain active" << std::endl;
	//OutputHandler* out = new OutputHandler();
	MqttOutput* out = MqttOutput::instance();
	//out->init("127.0.0.1",1883);
	//out->init("192.168.178.20",1883);
	out->init("broker.mqttdashboard.com",1883);
	//out->toogleDummy();

	if(argc == 2)
	{
		char* addr = argv[1];
		BluetoothDevice device("MetaWearDirect", addr);

		if(device.connect())
		{
			MetaWearDevice* mwDev = new MetaWearDevice(device, out);
			mwDev->bind();
			while(mwDev->getState() != BluetoothDevice::Disconnected) {
				base::Thread::sleep(1);
				if(mwDev->getState() == BluetoothDevice::Paired) break;
			}

			if(mwDev->getState() == BluetoothDevice::Paired) {
				mwDev->fetchData();
				while(mwDev->getState() != BluetoothDevice::Disconnected) base::Thread::sleep(1);
			}
			delete mwDev;
		}
	}
	else
	{
		BluetoothConnector connector;
		if(!connector.startDeviceDiscovery())
		{
			return 1;
		}

		std::list<MetaWearDevice*> mwDevices;
		BluetoothConnector::DeviceList devices = connector.getDevices();
		BluetoothConnector::DeviceList::iterator itDevice = devices.begin();

		for(;itDevice != devices.end(); itDevice++)
		{
			BluetoothDevice* blDev = *itDevice;
			if(blDev->getName() == "MetaWear")
			{
				MetaWearDevice* mwDev = new MetaWearDevice(*blDev, out);
				if(mwDev->connect())
				{
					mwDev->bind();
					while(mwDev->getState() == BluetoothDevice::Pairing) base::Thread::sleep(1);

					if(mwDev->getState() == BluetoothDevice::Paired)
					{
						mwDev->fetchData();
						mwDevices.push_back(mwDev);
						break;
					}
				}
			}
			delete *itDevice;
			*itDevice = NULL;
		}
		if(mwDevices.size() > 0) while(true) base::Thread::sleep(1);
	}
	return 0;
}
