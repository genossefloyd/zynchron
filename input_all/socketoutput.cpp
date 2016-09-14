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
    , m_socket(NULL)
    , m_server(new QTcpServer(this))
    , m_dummyThread(NULL)
{
    m_server->listen(QHostAddress::LocalHost, 54345);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(connected()));
}

void SocketOutput::connected()
{
    m_socket = m_server->nextPendingConnection();
    qDebug() << "SocketOutput is connected to " << m_socket->peerAddress();
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

    connect(m_dummyThread, SIGNAL(finished()), this, SLOT(stopDummy()));
    connect(m_dummyThread, SIGNAL(send(char,char,QByteArray)), this, SLOT(sendData(char,char,QByteArray)));
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

void SocketOutput::sendData(char deviceID, char tag, const QByteArray& payload)
{
    if(payload.size() > 255)
    {
        qWarning() << "trying to send a package of " << payload.size() << "bytes";
        return;
    }
    char length = (char)payload.size();

    QByteArray data;
    data.append(deviceID);
    data.append(tag);
    data.append(length);
    data.append(payload);

    if(m_socket && m_socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "sending: " << data.toHex();
        if( -1 == m_socket->write(data) )
            qWarning() << "Failed to send data to " << m_socket->peerAddress();
    }
    else
    {
        qDebug() << "not sending: " << data.toHex();
    }
}

void DummyDataSource::run()
{
    keepRunning = true;

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

        emit send(0x01, (char) msg::ACCELEROMETER, dummyData);
        dummyData.clear();
        x+=0.1f;
        y+=0.1f;
        z+=0.1f;
        QThread::msleep(100);
    }
}
