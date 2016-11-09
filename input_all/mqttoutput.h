#ifndef MQTTOUTPUT_H
#define MQTTOUTPUT_H

#include <mosquitto.h>

#include "abstractoutput.h"

class MqttOutput : public AbstractOutput
{
    Q_OBJECT
public:
    static MqttOutput* instance();

    void init(QString address,int port);
    bool shutdown();

private:
    explicit MqttOutput(QObject *parent = 0);

signals:

public slots:
    void sendData(char deviceID, char tag, const QByteArray& payload);

private:
    struct mosquitto* m_mqtt;
    constexpr static const char* ID  = "Zynchron";
    constexpr static const char* DEVICE_TYPE  = "Metawear";
};

#endif // MQTTOUTPUT_H
