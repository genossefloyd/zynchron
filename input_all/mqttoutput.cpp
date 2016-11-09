#include "mqttoutput.h"

#include <QDebug>

MqttOutput* MqttOutput::instance()
{
    static MqttOutput* instance = NULL;
    if( NULL == instance )
    {
        instance = new MqttOutput();
    }
    return instance;
}

MqttOutput::MqttOutput(QObject *parent) : AbstractOutput(parent)
{
    mosquitto_lib_init();
}

void MqttOutput::init(QString address,int port)
{
    m_mqtt = mosquitto_new(ID,true,0);

    qDebug() << "MqttOutput tries to connect to Broker on " <<  address << ":" << port;
    if( mosquitto_connect(m_mqtt,address.toStdString().c_str(),port,60) != MOSQ_ERR_SUCCESS )
    {
        qDebug() << "MqttOutput connection failed";
        return;
    }
    qDebug() << "MqttOutput is running";

    mosquitto_publish(m_mqtt,NULL,"DeviceType",strlen(DEVICE_TYPE),DEVICE_TYPE,0,true);
    mosquitto_loop_start(m_mqtt);
}

void MqttOutput::sendData(char deviceID, char tag, const QByteArray& payload)
{
    QString topic = DEVICE_TYPE;
    topic.append("/");
    topic.append(deviceID);
    topic.append("/data/");
    topic.append(tag);

    qDebug() << "sending mqtt: topic="<< topic << "; data= " << payload.toHex();
    mosquitto_publish(m_mqtt,NULL,topic.toStdString().c_str(),payload.length(),payload.data(),0,true);
}

bool MqttOutput::shutdown()
{
    if(mosquitto_disconnect(m_mqtt) == MOSQ_ERR_SUCCESS)
    {
        mosquitto_loop_stop(m_mqtt,false);
        qDebug() << "MqttOutput has stopped";
        mosquitto_reinitialise(m_mqtt, ID, false, 0);
        return true;
    }
    return false;
}
