#ifndef SOCKETOUTPUT_H
#define SOCKETOUTPUT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

#include "abstractoutput.h"

class SocketOutput : public AbstractOutput
{
    Q_OBJECT

private:
    explicit SocketOutput(QObject *parent = 0);
    QTcpSocket *m_socket;
    QTcpServer *m_server;

public:
    static SocketOutput* instance();

public slots:
    void connected();
    void sendData(char deviceID, char tag, const QByteArray& payload);
};

#endif // SOCKETOUTPUT_H
