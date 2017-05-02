#ifndef METAWEARDEVICE_H
#define METAWEARDEVICE_H

#include "bluetoothdevice.h"
#include "abstractoutput.h"
#include "base/semaphore.h"

#include "core/metawearboard.h"
#include "core/cpp/metawearboard_def.h"
#include "core/data.h"

#include <deque>
#include <mutex>

class MetaWearDevice : public BluetoothDevice
{
public:
    enum Signal {
        SWITCH,
        ACCELEROMETER,
        AMBIENT_LIGHT,
        GYRO
    };

    explicit MetaWearDevice(BluetoothDevice & device, AbstractOutput* output);
    virtual ~MetaWearDevice();

    bool bind();
    void unbind();
	
    void toogleLED();
    void fetchData();

private:
    enum MessageType 
    {
    	INIT,
        NOTIFY,
        FETCH_DATA,
        TOGGLE_LED
    };
	
	struct Message
	{
        MessageType             type;
		uuid_t					characteristic;
		std::vector<uint8_t>	payload;

        Message(const uuid_t& uuid, const uint8_t* data, size_t data_length)
        : type(NOTIFY), characteristic(uuid), payload(data, data+data_length) {}

        Message(MessageType msgType) 
        : type(msgType) { memset(&characteristic, 0, sizeof(characteristic)); }
	};

	struct MblMwMetaWearBoardCustom : public MblMwMetaWearBoard
	{
		MetaWearDevice*			parent;
		uint8_t                 datastreamid;
		bool                    fetchingdata;
	};

	static const MblMwBtleConnection CONNECTION;

    MblMwMetaWearBoardCustom*   m_board;
    AbstractOutput* 			m_output;
	
    bool						m_isProcessing;
	std::thread					m_workerThread;
	std::deque<Message*>		m_messageQueue;
	base::Semaphore				m_msgQueueSemaphore;
	std::condition_variable		m_cv;
    mutable std::mutex 			m_mutexNotify;

private:
    void execute();
    void add_message(Message* newMessage);
    void process_message(Message* newMessage);

	static MblMwMetaWearBoardCustom* mwbc_create(MetaWearDevice* parent);

    static void start_processing(MetaWearDevice* device);
    static void is_initialized(MblMwMetaWearBoard *board, int32_t status);
    
    static void handle_indication(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);
    static void add_notification(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);

    static void write_gatt_char(const void* caller, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
    static void read_gatt_char(const void* caller, const MblMwGattChar *characteristic);

    static void forward_data(const MblMwMetaWearBoardCustom* caller, const MblMwData* data, Signal signal);
    static void acc_handler(const void* caller, const MblMwData* data)
    {
        forward_data((MblMwMetaWearBoardCustom*)caller, data, Signal::ACCELEROMETER);
    }
};

uuid_t HighLow2Uuid(const uint64_t high, const uint64_t low);
void Uuid2HighLow(uuid_t uuid, uint64_t &high, uint64_t &low);

#endif // METAWEARDEVICE_H
