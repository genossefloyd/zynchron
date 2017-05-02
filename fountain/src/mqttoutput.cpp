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
    mosquitto_lib_init();
}

void MqttOutput::init(const std::string& address, int port)
{
    m_mqtt = mosquitto_new(ID,true,0);

    std::cout << "MqttOutput tries to connect to Broker on " <<  address << ":" << port << std::endl;
    if( mosquitto_connect(m_mqtt,address.c_str(),port,60) != MOSQ_ERR_SUCCESS )
    {
        std::cout << "MqttOutput connection failed" << std::endl;
        return;
    }
    std::cout << "MqttOutput is running" << std::endl;

    mosquitto_publish(m_mqtt,NULL,"DeviceType",strlen(DEVICE_TYPE),DEVICE_TYPE,0,true);
    mosquitto_loop_start(m_mqtt);
}

static float toFloat(char* array)
{
    float result = *(reinterpret_cast<float*>(array));
    return result;
}

static std::string value2String(int value)
{
	char element[8];
	int length = sprintf(element,"%x", value);
	return std::string(element, length);
}

void MqttOutput::sendData(char deviceID, char tag, const ByteArray& payload)
{
	if(payload.size() != 12)
	{
		return;
	}

    unsigned char id = deviceID;
    std::string topicByte = DEVICE_TYPE;
    topicByte.append("/");
    topicByte.append(value2String(id));
    topicByte.append("/byte/");
    topicByte.append(value2String(tag));

    std::string topicString = DEVICE_TYPE;
    topicString.append("/");
    topicString.append(value2String(id));
    topicString.append("/string/");
    topicString.append(value2String(tag));

    char* data = (char*)payload.data();
    int length = asprintf(&data,"%f,%f,%f",toFloat(data), toFloat(data+4), toFloat(data+8));

    std::cout << "sending mqtt: topic="<< topicByte << " data= " << std::hex << payload.data() << std::dec << std::endl;
    //std::cout << "sending mqtt: topic="<< topicString << " data= " << data << std::endl;

    mosquitto_publish(m_mqtt,NULL,topicByte.c_str(),payload.size(),payload.data(),0,false);
    mosquitto_publish(m_mqtt,NULL,topicString.c_str(),length,data,0,false);
    free(data);
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
