#ifndef MQTTOUTPUT_H
#define MQTTOUTPUT_H

#include <mosquitto.h>

#include "outputhandler.h"

//implementation of interface "AbstractOutput" to send data using the MQTT Protocol
//implementation is based on usage of 'mosquitto' library
//deriving of class OutputHandler to enable dummy data generation to test output functions
//using the singleton pattern
//
// This output handler sends data via MQTT protocol with a topic of certain schema:
//		<device_type>/<device_id>/<data_format>/<data_tag>
//
// Device type is always "Metawear". Data format can be "byte" or "string".
// Device id and data tag are submitted from data source together with the payload.
//
class MqttOutput : public OutputHandler
{
public:
	//singleton factory function
	//retrieve pointer to sole instance of this class
	//if instance does not exist yet, it is created
    static MqttOutput* instance();

    //initialise connection to MQTT broker with given address and port
    bool init(const std::string& address, int port);
    //shutdown connection to MQTT broker
    bool shutdown();

    //implement own version interface class to send data via MQTT protocol to connected broker
    //overrides test implementation of OutputHandler
    //
    //expects payload to be an array of 12 byte consisting of 3 floating point numbers
    //sends two MQTT messages with the provided data in different formats:
    //1. sends raw byte array with topic: Metawear/<device_id>/byte/<data_tag>
    //2. sends byte array as string with three comma separated values and topic: Metawear/<device_id>/string/<data_tag>
    void sendData(char deviceID, char tag, const ByteArray& payload) override;

private:
    //private ctor for singleton pattern
    explicit MqttOutput();

private:
    struct mosquitto* m_mqtt; //connection context handler struct from mosquitto library

    constexpr static const char* ID  = "Zynchron"; //MQTT client id
    constexpr static const char* DEVICE_TYPE  = "Metawear"; //identifier for data source device
};

#endif // MQTTOUTPUT_H
