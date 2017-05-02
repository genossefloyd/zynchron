#include "bluetoothconnector.h"
#include "core/metawearboard.h"

#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <unistd.h>

#include <thread>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "gattlib.h"

#include <iostream>

#define LE_SCAN_PASSIVE                 0x00
#define LE_SCAN_ACTIVE                  0x01

/* These LE scan and inquiry parameters were chosen according to LE General
 * Discovery Procedure specification.
 */
#define DISCOV_LE_SCAN_WIN              0x12
#define DISCOV_LE_SCAN_INT              0x12

#define EIR_NAME_SHORT     0x08  /* shortened local name */
#define EIR_NAME_COMPLETE  0x09  /* complete local name */

#define BLE_EVENT_TYPE     0x05
#define BLE_SCAN_RESPONSE  0x00//0x04

#define BLE_SCAN_TIMEOUT   4

static std::string parse_name(uint8_t* data, size_t size)
{
	size_t offset = 0;

	while (offset < size) {
		uint8_t field_len = data[0];
		size_t name_len;

		if (field_len == 0 || offset + field_len > size)
			return NULL;

		switch (data[1]) {
		case EIR_NAME_SHORT:
		case EIR_NAME_COMPLETE:
			name_len = field_len - 1;
			if (name_len > size)
				return "";

			return std::string((const char*)(data + 2), name_len);
		}

		offset += field_len + 1;
		data += field_len + 1;
	}
	return "";
}

BluetoothConnector::BluetoothConnector()
: m_deviceDescriptor(-1)
{
}

BluetoothConnector::~BluetoothConnector()
{
	if(m_deviceDescriptor >= 0)
	{
		disableScan(m_deviceDescriptor);
		hci_close_dev(m_deviceDescriptor);
	}
}

BluetoothConnector::DeviceList BluetoothConnector::getDevices()
{
	return m_devices;
}

int BluetoothConnector::enableScan(int device_desc)
{
	uint16_t interval = htobs(DISCOV_LE_SCAN_INT);
	uint16_t window = htobs(DISCOV_LE_SCAN_WIN);
	uint8_t own_address_type = 0x00;
	uint8_t filter_policy = 0x00;

	int ret = hci_le_set_scan_parameters(device_desc, LE_SCAN_ACTIVE, interval, window, own_address_type, filter_policy, 10000);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Set scan parameters failed (are you root?).\n");
		return 1;
	}

	ret = hci_le_set_scan_enable(device_desc, 0x01, 1, 10000);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Enable scan failed.\n");
		return 1;
	}

	return 0;
}

int BluetoothConnector::disableScan(int device_desc)
{
	if (device_desc == -1) {
		fprintf(stderr, "ERROR: Could not disable scan, not enabled yet.\n");
		return 1;
	}

	int result = hci_le_set_scan_enable(device_desc, 0x00, 1, 10000);
	if (result < 0) {
		fprintf(stderr, "ERROR: Disable scan failed.\n");
	}
	return result;
}

BluetoothConnector::DeviceList BluetoothConnector::startScan(int device_desc, int timeout)
{
	struct hci_filter old_options;
	socklen_t slen = sizeof(old_options);
	struct hci_filter new_options;
	unsigned char buffer[HCI_MAX_EVENT_SIZE+1];
	memset(buffer, HCI_MAX_EVENT_SIZE+1, 0);

	evt_le_meta_event* meta = (evt_le_meta_event*)(buffer + HCI_EVENT_HDR_SIZE + 1);
	le_advertising_info* info;
	char addr[18];
	int len;

	DeviceList devList;
	if (getsockopt(device_desc, SOL_HCI, HCI_FILTER, &old_options, &slen) < 0) {
		fprintf(stderr, "ERROR: Could not get socket options.\n");
		return devList;
	}

	hci_filter_clear(&new_options);
	hci_filter_set_ptype(HCI_EVENT_PKT, &new_options);
	hci_filter_set_event(EVT_LE_META_EVENT, &new_options);

	if (setsockopt(device_desc, SOL_HCI, HCI_FILTER,
				   &new_options, sizeof(new_options)) < 0) {
		fprintf(stderr, "ERROR: Could not set socket options.\n");
		return devList;
	}

	while (1) {
		struct pollfd fds;
		fds.fd     = device_desc;
		fds.events = POLLIN;

		int err = poll(&fds, 1, timeout * 1000);
		if (err <= 0) {
			break;
		} else if ((fds.revents & POLLIN) == 0) {
			continue;
		}

		len = read(device_desc, buffer, sizeof(buffer));

		for(int i = 0; i < len; i++) { printf("%02X",buffer[i]);}
		printf("\n");
		for(int i = 0; i < len; i++) { printf("%c",buffer[i]);}
		printf("\n");

		if (meta->subevent != 0x02 || (uint8_t)buffer[BLE_EVENT_TYPE] != BLE_SCAN_RESPONSE)
			continue;

		info = (le_advertising_info*) (meta->data + 1);
		ba2str(&info->bdaddr, addr);

		std::string name = parse_name(info->data, info->length);

		printf("found device: %s\n", addr);
		if(name == "MetaWear")
		{
			BluetoothDevice* device = new BluetoothDevice(addr, name);
			device->connect();
			devList.push_back(device);
		}
	}

	setsockopt(device_desc, SOL_HCI, HCI_FILTER, &old_options, sizeof(old_options));
	return devList;
}

bool BluetoothConnector::startDeviceDiscovery()
{
    std::cout << "Scanning for devices..." << std::endl;
    int dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
		fprintf(stderr, "ERROR: Invalid device.\n");
		return false;
	}

	int deviceDesc = hci_open_dev(dev_id);
	if (deviceDesc < 0) {
		fprintf(stderr, "ERROR: Could not open device.\n");
		return false;
	}
	m_deviceDescriptor = deviceDesc;

	bool success = true;
	{
		enableScan(m_deviceDescriptor);

		m_devices = startScan(m_deviceDescriptor, BLE_SCAN_TIMEOUT);
		if (m_devices.empty()) {
			fprintf(stderr, "ERROR: Advertisement fail.\n");
			success = false;
		}
	}
	puts("Scan completed");
	return success;
}

