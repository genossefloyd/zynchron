#include "mqttoutput.h"
#include "bluetoothconnector.h"
#include "metaweardevice.h"

#include <iostream>
#include <string>

//helper struct to collect configuration given via parameter
struct Parameter
{
	bool 			isDummy;
	std::string 	addrDevice;
	std::string 	addrMqtt;
	unsigned int 	portMqtt;

	Parameter() : isDummy(false), portMqtt(1883) {};
};

//parse arguments and return values via 'Parameter' struct
bool parseArguments(int argc, char *argv[], Parameter& parameter)
{
	if(argc <= 1)
		return false;

	bool ok = true;
	for(int idx = 0; idx < argc; idx++)
	{
		//if set, dummy data shall be used
		std::string arg(argv[idx]);
		if(arg == "--dummy")
		{
			parameter.isDummy = true;
			continue;
		}

		//define port to mqtt broker
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

		//define address of mqtt broker as output target
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

		//define mac address of bluetooth device to connect to (will skip bluetooth discovery)
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
	//parse input/configuration parameter
	Parameter params;
	if(!parseArguments(argc, argv, params))
	{
		std::cout << "Invalid arguments!" << std::endl;
		return 1;
	}

	//connect to mqtt broker
	std::cout << "Data Fountain active" << std::endl;
	std::string addrMqtt = "broker.mqttdashboard.com";
	if(!params.addrMqtt.empty())
	{
		addrMqtt = params.addrMqtt;
	}

	MqttOutput* out = MqttOutput::instance();
	out->init(addrMqtt, params.portMqtt);

	//if dummy mode, start dummy data generation
	if(params.isDummy)
	{
		out->toogleDummy();
		while(true) base::Thread::sleep(1);
	}


	MetaWearDevice* mwDev = NULL;
	//if no bluetooth mac address is given, start bluetooth discovery
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
				//connect to found metawear device
				mwDev = new MetaWearDevice(*blDev, out);
				if(mwDev->connect()) //try to connect to device (bluetooth only)
				{
					mwDev->bind(); //start metawear binding process (asynchronous)
					//wait until binding process is not running anymore
					while(mwDev->getState() == BluetoothDevice::Pairing) base::Thread::sleep(1);

					//check for success
					if(mwDev->getState() == BluetoothDevice::Paired)
					{
						//if success, start fetching data -
						mwDev->fetchData();
					}
					else
					{
						//if binding failed, clear metawear device
						delete mwDev;
						mwDev = NULL;
					}
				}
			}
			//free bluetooth device
			//if a metawear device was derived, this connection credentials have been transfered
			delete *itDevice;
			*itDevice = NULL;
		}
	}
	else //if a mac address is given, try to connect to this bluetooth device right away
	{
		//try direct connection to a potential metaware device
		BluetoothDevice device("MetaWearDirect", params.addrDevice);

		if(device.connect())
		{
			mwDev = new MetaWearDevice(device, out);
			mwDev->bind();
			while(mwDev->getState() != BluetoothDevice::Disconnected) {
				base::Thread::sleep(1);
				//wait for finished metawear binding process
				if(mwDev->getState() == BluetoothDevice::Paired) break;
			}

			if(mwDev->getState() == BluetoothDevice::Paired) {
				//start fetching data from metawear board
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
