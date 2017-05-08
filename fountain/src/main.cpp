#include "mqttoutput.h"
#include "bluetoothconnector.h"
#include "metaweardevice.h"

#include <iostream>
#include <string>

struct Parameter
{
	bool 			isDummy;
	std::string 	addrDevice;
	std::string 	addrMqtt;
	unsigned int 	portMqtt;

	Parameter() : isDummy(false), portMqtt(1883) {};
};

bool parseArguments(int argc, char *argv[], Parameter& parameter)
{
	if(argc <= 1)
		return false;

	bool ok = true;
	for(int idx = 0; idx < argc; idx++)
	{
		std::string arg(argv[idx]);
		if(arg == "--dummy")
		{
			parameter.isDummy = true;
			continue;
		}

		if(arg == "-p")
		{
			idx++;
			if(idx < argc)
			{
				parameter.portMqtt = (unsigned int) atoi(argv[idx]);
				continue;
			}
			else
			{
				ok = false;
				break;
			}
		}

		if(arg == "-o")
		{
			idx++;
			if(idx < argc)
			{
				parameter.addrMqtt = argv[idx];
				continue;
			}
			else
			{
				ok = false;
				break;
			}
		}

		if(arg == "-d")
		{
			idx++;
			if(idx < argc)
			{
				parameter.addrDevice = argv[idx];
				continue;
			}
			else
			{
				ok = false;
				break;
			}
		}
	}

	return ok;
}

int main(int argc, char *argv[])
{
	Parameter params;
	if(!parseArguments(argc, argv, params))
	{
		std::cout << "Invalid arguments!" << std::endl;
		return 1;
	}

	std::cout << "Data Fountain active" << std::endl;
	std::string addrMqtt = "broker.mqttdashboard.com";
	if(!params.addrMqtt.empty())
	{
		addrMqtt = params.addrMqtt;
	}

	//OutputHandler* out = new OutputHandler();
	MqttOutput* out = MqttOutput::instance();
	out->init(addrMqtt, params.portMqtt);

	if(params.isDummy)
	{
		out->toogleDummy();
		while(true) base::Thread::sleep(1);
	}

	MetaWearDevice* mwDev = NULL;
	if(params.addrDevice.empty())
	{
		//scan for BLE devices
		BluetoothConnector connector;
		if(!connector.startDeviceDiscovery())
		{
			return 1;
		}

		BluetoothConnector::DeviceList devices = connector.getDevices();
		BluetoothConnector::DeviceList::iterator itDevice = devices.begin();

		//check if we have found a metawear device
		for(;itDevice != devices.end(); itDevice++)
		{
			BluetoothDevice* blDev = *itDevice;
			if(mwDev != NULL && blDev->getName() == "MetaWear")
			{
				mwDev = new MetaWearDevice(*blDev, out);
				if(mwDev->connect())
				{
					mwDev->bind();
					while(mwDev->getState() == BluetoothDevice::Pairing) base::Thread::sleep(1);

					if(mwDev->getState() == BluetoothDevice::Paired)
					{
						mwDev->fetchData();
					}
					else
					{
						delete mwDev;
						mwDev = NULL;
					}
				}
			}
			delete *itDevice;
			*itDevice = NULL;
		}
	}
	else
	{
		//try direct connection to a potential metaware device
		BluetoothDevice device("MetaWearDirect", params.addrDevice);

		if(device.connect())
		{
			mwDev = new MetaWearDevice(device, out);
			mwDev->bind();
			while(mwDev->getState() != BluetoothDevice::Disconnected) {
				base::Thread::sleep(1);
				if(mwDev->getState() == BluetoothDevice::Paired) break;
			}

			if(mwDev->getState() == BluetoothDevice::Paired) {
				mwDev->fetchData();
			}
		}
	}

	if(mwDev != NULL)
	{
		while(mwDev->getState() != BluetoothDevice::Disconnected) base::Thread::sleep(1);
		delete mwDev;
	}
	return 0;
}
