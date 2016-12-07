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

static float toFloat(QByteArray array)
{
    float result = 0;
    if(array.size() == 4)
        result = *(reinterpret_cast<float*>(array.data()));
    return result;
}

void MqttOutput::sendData(char deviceID, char tag, const QByteArray& payload)
{
    QString topicByte = DEVICE_TYPE;
    topicByte.append("/");
    topicByte.append(QString::asprintf("%x",deviceID));
    topicByte.append("/byte/");
    topicByte.append(QString::asprintf("%x",tag));

    QString topicString = DEVICE_TYPE;
    topicString.append("/");
    topicString.append(QString::asprintf("%x",deviceID));
    topicString.append("/string/");
    topicString.append(QString::asprintf("%x",tag));

    QString data = QString::asprintf("%f,%f,%f",toFloat(payload.mid(0,4)),toFloat(payload.mid(4,4)),toFloat(payload.mid(8,4)));

    qDebug() << "sending mqtt: topic="<< topicByte << " data= " << payload.toHex();
    qDebug() << "sending mqtt: topic="<< topicString << " data= " << data;

    mosquitto_publish(m_mqtt,NULL,topicByte.toStdString().c_str(),payload.length(),payload.data(),0,true);
    mosquitto_publish(m_mqtt,NULL,topicString.toStdString().c_str(),data.length(),data.toStdString().c_str(),0,true);
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
