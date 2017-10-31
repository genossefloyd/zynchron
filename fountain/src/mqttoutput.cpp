#include "mqttoutput.h"
#include <iostream>
#include <cstring>

MqttOutput* MqttOutput::instance()
{
    static MqttOutput* instance = NULL;
    if( NULL == instance )
    {
        instance = new MqttOutput();
    }
    return instance;
}

MqttOutput::MqttOutput()
: m_mqtt(NULL)
{
	//initialise library
    mosquitto_lib_init();
}

bool MqttOutput::init(const std::string& address, int port)
{
	//if no connection context is available yet, create a new one
    if(m_mqtt == nullptr)
    {
    	//provide client ID, request a clean session, do not provide pointer to custom context argument
    	if(0 == (m_mqtt = mosquitto_new(ID,true,0)))
    	{
    		std::cout << "MqttOutput initialisation failed" << std::endl;
    		return false;
    	}
    }

    //connect to broker with given connection infos
    std::cout << "MqttOutput tries to connect to Broker on " <<  address << ":" << port << std::endl;
    if( mosquitto_connect(m_mqtt,address.c_str(),port,60) != MOSQ_ERR_SUCCESS )
    {
        std::cout << "MqttOutput connection failed" << std::endl;
        return false;
    }
    std::cout << "MqttOutput is running" << std::endl;

    //send first, retaining message with type identifier/name of data source device
    mosquitto_publish(m_mqtt,NULL,"DeviceType",strlen(DEVICE_TYPE),DEVICE_TYPE,0,true);
    //start mqtt message handling thread
    mosquitto_loop_start(m_mqtt);
    return true;
}

bool MqttOutput::shutdown()
{
    if(mosquitto_disconnect(m_mqtt) == MOSQ_ERR_SUCCESS)
    {
        mosquitto_loop_stop(m_mqtt,false);
        std::cout << "MqttOutput has stopped" << std::endl;
        mosquitto_reinitialise(m_mqtt, ID, false, 0);
        return true;
    }
    return false;
}

//helper function to interpret a byte array as a floating point number
//array needs to be 4 bytes long
static float toFloat(char* array)
{
    float result = *(reinterpret_cast<float*>(array));
    return result;
}

//convert an integer number (4 bytes) into hexadecimal string representation
static std::string value2String(int value)
{
	char element[8];
	int length = sprintf(element,"%x", value);
	return std::string(element, length);
}

void MqttOutput::sendData(char deviceID, char tag, const ByteArray& payload)
{
	//check for valid array length
	if(payload.size() != 12)
	{
		return;
	}

    unsigned char id = deviceID;
    //construct topic for byte message
    std::string topicByte = DEVICE_TYPE;
    topicByte.append("/");
    topicByte.append(value2String(id));
    topicByte.append("/byte/");
    topicByte.append(value2String(tag));

    //construct topic for string message
    std::string topicString = DEVICE_TYPE;
    topicString.append("/");
    topicString.append(value2String(id));
    topicString.append("/string/");
    topicString.append(value2String(tag));

    //create payload string from raw byte array
    char* data = (char*)payload.data();
    int length = asprintf(&data,"%f,%f,%f",toFloat(data), toFloat(data+4), toFloat(data+8));

    //debug output
    std::cout << "sending mqtt: topic="<< topicString << " data= " << data << std::endl;

    //send payload in two messages of different format
    mosquitto_publish(m_mqtt,NULL,topicByte.c_str(),payload.size(),payload.data(),0,false);
    mosquitto_publish(m_mqtt,NULL,topicString.c_str(),length,data,0,false);
    free(data);
}
