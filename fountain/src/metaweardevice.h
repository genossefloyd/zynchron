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

//specialisation of bluetooth device to implement further connection handling for metawear devices
//uses metaware library
class MetaWearDevice : public BluetoothDevice
{
public:
	//enum for signal/data source types
    enum Signal {
        SWITCH,
        ACCELEROMETER,
        AMBIENT_LIGHT,
        GYRO
    };

    //ctor - copy connection info from existing, connected bluetooth device
    //take pointer to designated output class
    explicit MetaWearDevice(BluetoothDevice & device, AbstractOutput* output);
    virtual ~MetaWearDevice();

    //start binding/connection progress to metawear device
    //binding/initialisation process and message handling is done in a separate thread
    //function returns immediately after starting the binding process and does not wait for its completion
    //returns true if binding process is started successfully
    bool bind();
    //disconnect from metawear device (without closing bluetooth connection)
    void unbind();
	
    void toogleLED();
    void fetchData();

private:
    //known message/command types
    enum MessageType 
    {
    	INIT, //init connection to metawear board
        NOTIFY, //notification by metawear board about changed value
        FETCH_DATA, //command to toggle data collection from metawear boad
        TOGGLE_LED //command to turn on and off the LED on the metawear board
    };
	
    //message object to issue and handle commands send to the metawear board asynchronously
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

	//helping construct to add further information to a MblMwMetaWearBoard object
	//this way it is possible to forward custom information into own callback functions through the metawear library
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
    //main function of worker thread to handle any received messages via message queue
    void execute();
    void add_message(Message* newMessage);
    void process_message(Message* newMessage);

	static MblMwMetaWearBoardCustom* mwbc_create(MetaWearDevice* parent);

    static void start_processing(MetaWearDevice* device);
    static void is_initialized(MblMwMetaWearBoard *board, int32_t status);
    
    static void handle_indication(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);
    //implementation of callback function for bluetooth connection which is called if new data can be read via bluetooth
    static void add_notification(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);

    //implementation of interface function to send GATT package via bluetooth
    static void write_gatt_char(const void* caller, const MblMwGattChar *characteristic, const uint8_t *value, uint8_t length);
    //implementation of interface function to read GATT package via bluetooth
    static void read_gatt_char(const void* caller, const MblMwGattChar *characteristic);

    //function to format raw data from metawear board and forward it to the output handler
    static void forward_data(const MblMwMetaWearBoardCustom* caller, const MblMwData* data, Signal signal);
    static void acc_handler(const void* caller, const MblMwData* data)
    {
        forward_data((MblMwMetaWearBoardCustom*)caller, data, Signal::ACCELEROMETER);
    }
};

//convert metawear uuid format to gattlib uuid format
uuid_t HighLow2Uuid(const uint64_t high, const uint64_t low);
//convert gattlib uuid format to metawear uuid format
void Uuid2HighLow(uuid_t uuid, uint64_t &high, uint64_t &low);

#endif // METAWEARDEVICE_H
