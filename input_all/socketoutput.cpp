#include "socketoutput.h"
#include "messages.h"

#include <QDebug>

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
    : AbstractOutput(parent)
    , m_socket(NULL)
    , m_server(new QTcpServer(this))
{
    m_server->listen(QHostAddress::LocalHost, 54345);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(connected()));
}

void SocketOutput::connected()
{
    m_socket = m_server->nextPendingConnection();
    qDebug() << "SocketOutput is connected to " << m_socket->peerAddress();
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

