#include "socketoutput.h"
#include "messages.h"

#include <qdebug.h>
#include <qmath.h>

SocketOutput* SocketOutput::instance()
{
    static SocketOutput* instance = NULL;
    if( NULL == instance )
    {
        instance = new SocketOutput();
    }
    return instance;
}

SocketOutput::SocketOutput(QObject *parent)
    : QObject(parent)
    , m_dummyThread(NULL)
{
    m_socket = new QUdpSocket(this);
}

void SocketOutput::toogleDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();
    else
        startDummy();
}

void SocketOutput::startDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();

    m_dummyThread = new DummyDataSource();
    connect(m_dummyThread, SIGNAL(finished), this, SLOT(stopDummy));
    m_dummyThread->start();
}

void SocketOutput::stopDummy()
{
    if(m_dummyThread)
    {
        m_dummyThread->keepRunning = false;
        m_dummyThread->wait();
        delete m_dummyThread;
        m_dummyThread = NULL;
    }
}

bool SocketOutput::sendData(char deviceID, char tag, const QByteArray& payload)
{
    if(payload.size() > 255)
    {
        qWarning() << "trying to send a package of " << payload.size() << "bytes";
        return false;
    }
    char length = (char)payload.size();

    QByteArray data;
    data.append(deviceID);
    data.append(tag);
    data.append(length);
    data.append(payload);

    qDebug() << "sending: " << data.toHex();
    return -1 != m_socket->writeDatagram(data.data(), data.size(), QHostAddress::Broadcast, 54345);
}

void DummyDataSource::run()
{
    keepRunning = true;

    SocketOutput* out = SocketOutput::instance();
    if( NULL == out )
        return;

    float x = 0.0f, y = 1.57f, z = 3.14f;
    int len = sizeof(float);
    QByteArray dummyData;

    while(keepRunning)
    {
        float value = qSin(x);
        dummyData.append((char*)&value,len);
        value = qSin(y);
        dummyData.append((char*)&value,len);
        value = qSin(z);
        dummyData.append((char*)&value,len);

        out->sendData(0x01, (char) msg::ACCELEROMETER, dummyData);
        dummyData.clear();
        x+=0.1f;
        y+=0.1f;
        z+=0.1f;
    }
}
