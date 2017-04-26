#ifndef MQTTOUTPUT_H
#define MQTTOUTPUT_H

#include <mosquitto.h>

#include "outputhandler.h"

class MqttOutput : public OutputHandler
{
public:
    static MqttOutput* instance();

    void init(const std::string& address, int port);
    bool shutdown();

    void sendData(char deviceID, char tag, const ByteArray& payload);

private:
    explicit MqttOutput();

private:
    struct mosquitto* m_mqtt;
    constexpr static const char* ID  = "Zynchron";
    constexpr static const char* DEVICE_TYPE  = "Metawear";
};

#endif // MQTTOUTPUT_H
