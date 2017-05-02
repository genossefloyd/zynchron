#include "metaweardevice.h"
#include "messages.h"

#include "base/MutexGuard.h"
#include "base/Thread.h"
#include "bluetooth/uuid.h"

#include "core/cpp/constant.h"
#include "core/status.h"
#include "core/settings.h"
#include "core/module.h"
#include "core/datasignal.h"
#include "core/types.h"
#include "sensor/accelerometer.h"
#include "sensor/accelerometer_bosch.h"
#include "sensor/accelerometer_mma8452q.h"
#include "peripheral/led.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define PRINT_ARRAY(data, len) for(size_t j = 0; j < len; j++){ printf("%02X", data[j]); } printf("\n");

static const uuid_t METAWARE_SERVICE_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.service_uuid_low);

static const uuid_t METAWARE_CHARACTERISTIC_UUID = HighLow2Uuid(METAWEAR_SERVICE_NOTIFY_CHAR.uuid_high,
                                                    METAWEAR_SERVICE_NOTIFY_CHAR.uuid_low);

const MblMwBtleConnection MetaWearDevice::CONNECTION = { MetaWearDevice::write_gatt_char, MetaWearDevice::read_gatt_char };

uuid_t HighLow2Uuid(const uint64_t high, const uint64_t low)
{
	char buffer[33];
	int len = sizeof(buffer);
	memset(buffer,0,len);
	int wlen = snprintf(buffer, len, "%016" PRIx64 "%016" PRIx64, high, low);
	if(len <= wlen)
	{
		printf("Error");
	}

	std::string suuid(buffer);
	suuid.insert(20,"-");
	suuid.insert(16,"-");
	suuid.insert(12,"-");
	suuid.insert(8,"-");

	uuid_t uuid;
	memset(&uuid, 0, sizeof(uuid_t));
	gattlib_string_to_uuid(suuid.c_str(), suuid.size(), &uuid);
	return uuid;
}

static void uuid_to_bt_uuid(uuid_t* uuid, bt_uuid_t* bt_uuid) {
	memcpy(&bt_uuid->value, &uuid->value, sizeof(bt_uuid->value));
	if (uuid->type == SDP_UUID16) {
		bt_uuid->type = bt_uuid_t::BT_UUID16;
	} else if (uuid->type == SDP_UUID32) {
		bt_uuid->type = bt_uuid_t::BT_UUID32;
	} else if (uuid->type == SDP_UUID128) {
		bt_uuid->type = bt_uuid_t::BT_UUID128;
	} else {
		bt_uuid->type = bt_uuid_t::BT_UUID_UNSPEC;
	}
}

void Uuid2HighLow(uuid_t uuid, uint64_t &high, uint64_t &low)
{
	char buffer[37];
	memset(buffer, 0, sizeof(buffer));

	bt_uuid_t btuuid;
	uuid_to_bt_uuid(&uuid,&btuuid);
	bt_uuid_to_string(&btuuid, buffer, sizeof(buffer) );
	for (uint32_t i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] == '-') buffer[i] = ' ';
	}

	std::istringstream iss(buffer);
	std::string c1, c2, p1, p2, p3, p4, p5;
	iss >> p1 >> p2 >> p3 >> p4 >> p5;
	c1 = p1 + p2 + p3;
	c2 = p4 + p5;
	sscanf(c1.c_str(), "%016" PRIx64, &high);
	sscanf(c2.c_str(), "%016" PRIx64, &low);
}

static int32_t check_acc_type(MblMwMetaWearBoard* board)
{
    auto acc_type = mbl_mw_metawearboard_lookup_module(board, MBL_MW_MODULE_ACCELEROMETER);
    switch(acc_type)
    {
    case MBL_MW_MODULE_TYPE_NA:
        // should never reach this case statement
        std::cout << "No accelerometer on this board" << std::endl;
        break;
    case MBL_MW_MODULE_ACC_TYPE_MMA8452Q:
    	std::cout << "Acc Type = MMA8452Q" << std::endl;
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMI160:
    	std::cout << "Acc Type = BMI160" << std::endl;
        break;
    case MBL_MW_MODULE_ACC_TYPE_BMA255:
    	std::cout << "Acc Type = BMA255" << std::endl;
        break;
    default:
    	std::cout << "Unknown type";
        break;
    }
    return acc_type;
}

MetaWearDevice::MetaWearDevice(BluetoothDevice & device, AbstractOutput* output)
    : BluetoothDevice(device)
    , m_board(NULL)
	, m_output(output)
{
	assert(output != NULL);
}

MetaWearDevice::~MetaWearDevice()
{
    m_board->reconnect = false;
    mwbc_disconnect(m_board);
    mbl_mw_metawearboard_free(m_board);
    m_board = NULL;
}

MetaWearDevice::MblMwMetaWearBoardCustom* MetaWearDevice::mwbc_create(MetaWearDevice* parent)
{
    MblMwMetaWearBoardCustom* board = new MblMwMetaWearBoardCustom();
    memcpy(&board->btle_conn, &CONNECTION, sizeof(MblMwBtleConnection));
    board->parent = parent;
    board->datastreamid = std::rand() & 0xFF;
    board->fetchingdata = false;
    board->reconnect = true;

    return board;
}

bool MetaWearDevice::initialize()
{
	if(m_board != NULL)
	{
		return false;
	}
	m_board = mwbc_create(this);
	m_state = Pairing;

    //mbl_mw_settings_set_connection_parameters(m_board, 10.f, 50.f, 0, 1000);
	gattlib_register_notification(m_connection, gatt_notification_cb, this);
	gattlib_notification_start(m_connection, &METAWARE_CHARACTERISTIC_UUID);

    mbl_mw_metawearboard_initialize(m_board, is_initialized);
    return true;
}

void MetaWearDevice::is_initialized(MblMwMetaWearBoard* caller, int32_t status)
{
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    std::cout << "Board status: " << status << std::endl;

    if(status != MBL_MW_STATUS_ERROR_TIMEOUT)
    {
        MblMwLedPattern pattern;
        mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_BLINK);
        pattern.repeat_count = 3;

        MblMwLedColor color = MBL_MW_LED_COLOR_RED;
        if(status == MBL_MW_STATUS_OK)
        {
            color = MBL_MW_LED_COLOR_GREEN;
        }
        mbl_mw_led_write_pattern(board, &pattern, color);
        mbl_mw_led_play(board);

        //setup acc sampling
        check_acc_type(board);
        auto acc_signal = mbl_mw_acc_get_acceleration_data_signal(board);
        mbl_mw_datasignal_subscribe(acc_signal, acc_handler);

        board->parent->m_state = Paired;
    }
    else
    {
        mwbc_disconnect(board);
    }
}

void MetaWearDevice::mwbc_disconnect(MblMwMetaWearBoardCustom *board)
{
	if(board->parent->m_state == Paired)
	{
	    mbl_mw_metawearboard_tear_down(board);
	    board->parent->m_state = Connected;
	}
}

void MetaWearDevice::write_gatt_char(const void* caller, const MblMwGattChar *characteristic,
        const uint8_t *value, uint8_t length)
{
	printf("write gatt char begin\n");
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    if(board->parent->m_state >= Connected)
    {
    	base::MutexGuard g(board->parent->m_mutexConnection);
		uuid_t uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

		printf("write "); PRINT_ARRAY(value, length);
		int ret = gattlib_write_char_by_uuid(board->parent->m_connection, &uuidChar, value, length);
		if(ret != 0)
		{
			fprintf(stderr, "Fail to find write char.\n");
		}
    }
	printf("write gatt char end\n");
}

void MetaWearDevice::read_gatt_char(const void* caller, const MblMwGattChar *characteristic)
{
	printf("read gatt char begin\n");
    MblMwMetaWearBoardCustom* board = (MblMwMetaWearBoardCustom*)caller;
    if(board->parent->m_state >= Connected)
    {
    	board->parent->m_mutexConnection.lock();
        uuid_t uuidChar = HighLow2Uuid(characteristic->uuid_high,characteristic->uuid_low);

		uint8_t buffer[255];
		size_t buf_len = sizeof(buffer);
		memset(buffer, 0, buf_len);

		if(0 != gattlib_read_char_by_uuid(board->parent->m_connection, &uuidChar, buffer, &buf_len))
		{
			buf_len = 0;
		}
		board->parent->m_mutexConnection.unlock();
		printf("read %d bytes\n", buf_len);

		mbl_mw_metawearboard_char_read(board, characteristic, buffer, buf_len);
    }
    printf("read gatt char end\n");
}

void MetaWearDevice::gatt_notification_cb(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data)
{
	printf("notify gatt char start\n");
	printf("received "); PRINT_ARRAY(data, data_length);
    if(gattlib_uuid_cmp(uuid, &METAWARE_CHARACTERISTIC_UUID) == 0)
    {
    	MetaWearDevice* device = (MetaWearDevice*) user_data;
    	base::MutexGuard g(device->m_mutexConnection);

    	auto func = [](MetaWearDevice *device, const uint8_t* buf, size_t bufLen) -> void {
    		//base::Thread::sleepMs(100);
    		mbl_mw_metawearboard_notify_char_changed(device->m_board, buf, bufLen);
    		printf("notify gatt char done\n");
    	};
    	std::thread test(func, device, data, data_length);
    	test.detach();
    }
    printf("notify gatt char end\n");
}

void MetaWearDevice::toogleLED()
{
    if( mbl_mw_metawearboard_is_initialized(m_board) )
    {
        static bool isOn = true;
        isOn = !isOn;

        MblMwLedPattern pattern;
        mbl_mw_led_load_preset_pattern(&pattern, MBL_MW_LED_PRESET_SOLID);
        mbl_mw_led_write_pattern(m_board, &pattern, MBL_MW_LED_COLOR_GREEN);
        pattern.repeat_count = 0;
        if(isOn)
            mbl_mw_led_stop(m_board);
        else
            mbl_mw_led_play(m_board);
    }
}

void MetaWearDevice::fetchData()
{
    std::cout<<"toggle data" << std::endl;
    if( mbl_mw_metawearboard_is_initialized(m_board) )
    {
        if( !m_board->fetchingdata )
        {
            m_board->fetchingdata = true;
            //enable acc sampling
            mbl_mw_acc_enable_acceleration_sampling(m_board);
            mbl_mw_acc_start(m_board);
        }
        else
        {
            m_board->fetchingdata = false;
            //enable acc sampling
            mbl_mw_acc_disable_acceleration_sampling(m_board);
            mbl_mw_acc_stop(m_board);
        }
    }
}

void MetaWearDevice::handle_data(const MblMwMetaWearBoardCustom* board, const MblMwData* data, Signal signal)
{
    ByteArray payload;
    float value;
    int len = sizeof(float);

    switch(signal)
    {
    case ACCELEROMETER:
        printf("(%.3f, %.3f, %.3f)\n",
               ((MblMwCartesianFloat*)data->value)->x,
               ((MblMwCartesianFloat*)data->value)->y,
               ((MblMwCartesianFloat*)data->value)->z);

        payload.resize(3*len);
        value = ((MblMwCartesianFloat*)data->value)->x;
        memcpy(payload.data(),(char*)&value,len);
        value = ((MblMwCartesianFloat*)data->value)->y;
        memcpy(payload.data()+len,(char*)&value,len);
        value = ((MblMwCartesianFloat*)data->value)->z;
        memcpy(payload.data()+(2*len),(char*)&value,len);

        if(board->parent)
        	board->parent->m_output->sendData(board->datastreamid, msg::ACCELEROMETER, payload);
        break;
    default:
        break;
    }
}
