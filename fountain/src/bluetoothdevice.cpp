#include "bluetoothdevice.h"

std::mutex BluetoothDevice::sm_mutexConnect;

BluetoothDevice::BluetoothDevice(const std::string& name, const std::string& addr)
    : m_deviceName(name)
	, m_deviceAddress(addr)
	, m_connection(NULL)
	, m_state(Disconnected)
{
}

BluetoothDevice::BluetoothDevice(BluetoothDevice& device)
    : m_deviceName(device.m_deviceName)
	, m_deviceAddress(device.m_deviceAddress)
	, m_connection(device.m_connection)
	, m_state(device.m_state)
{
	device.m_connection = NULL;
	device.m_state = Disconnected;
}

BluetoothDevice::~BluetoothDevice()
{
	disconnect();
}

std::string BluetoothDevice::getName() const
{
    return m_deviceName;
}

std::string BluetoothDevice::getAddress() const
{
    return m_deviceAddress;
}

void BluetoothDevice::disconnect()
{
	if(m_state == Connected)
	{
		gattlib_disconnect(m_connection);
		m_connection = NULL;
		m_state = Disconnected;
	}
}

bool BluetoothDevice::connect()
{
	if(m_state == Connected) {
		return true;
	}

	if (m_deviceName.size() > 0) {
		printf("Discovered %s - '%s'\n", m_deviceAddress.c_str(), m_deviceName.c_str());
	} else {
		printf("Discovered %s\n", m_deviceAddress.c_str());
	}

	m_state = Connecting;

	const char* addr = m_deviceAddress.c_str();
	gatt_connection_t* gatt_connection;

	printf("------------CONNECT %s ---------------\n", addr);

	gatt_connection = gattlib_connect(NULL, addr, BDADDR_LE_RANDOM, BT_SEC_LOW, 0, 0);
	if (gatt_connection == NULL) {
		gatt_connection = gattlib_connect(NULL, addr, BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	}

	if (gatt_connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		m_state = Disconnected;
		return false;
	} else {
		puts("Succeeded to connect to the bluetooth device.");
	}

	//discoverChars(gatt_connection);

	printf("------------DONE %s ---------------\n", addr);

	m_connection = gatt_connection;
	m_state = Connected;
	return true;
}

void BluetoothDevice::discoverChars(gatt_connection_t* gatt_connection)
{
	if(gatt_connection == NULL) {
		return;
	}
	char uuid_str[MAX_LEN_UUID_STR + 1];
	int ret;

	printf("--------------- DISCOVER ---------------\n");

	gattlib_primary_service_t* services;
	int services_count = 0;
	ret = gattlib_discover_primary(gatt_connection, &services, &services_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover primary services.\n");
		return;
	}

	for (int i = 0; i < services_count; i++) {
		gattlib_uuid_to_string(&services[i].uuid, uuid_str, sizeof(uuid_str));

		printf("service[%d] start_handle:%02x end_handle:%02x uuid:%s\n", i,
				services[i].attr_handle_start, services[i].attr_handle_end,
				uuid_str);
	}
	free(services);

	gattlib_characteristic_t* characteristics;
	int characteristics_count = 0;
	ret = gattlib_discover_char(gatt_connection, &characteristics, &characteristics_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover characteristics.\n");
		return;
	}
	for (int i = 0; i < characteristics_count; i++) {
		gattlib_uuid_to_string(&characteristics[i].uuid, uuid_str, sizeof(uuid_str));

		printf("characteristic[%d] properties:%02x value_handle:%04x uuid:%s\n", i,
				characteristics[i].properties, characteristics[i].value_handle,
				uuid_str);
	}
	free(characteristics);

	gattlib_descriptor_t* descriptors;
	int descriptor_count = 0;
	ret = gattlib_discover_desc(gatt_connection, &descriptors, &descriptor_count);
	if (ret != 0) {
		fprintf(stderr, "Fail to discover descriptors.\n");
		return;
	}
	for (int i = 0; i < descriptor_count; i++) {
		gattlib_uuid_to_string(&descriptors[i].uuid, uuid_str, sizeof(uuid_str));

		printf("descriptors[%d] value_handle:%04x uuid:%s\n", i,
				descriptors[i].handle,
				uuid_str);
	}
	free(descriptors);
}
