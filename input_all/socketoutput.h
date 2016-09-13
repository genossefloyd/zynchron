#ifndef SOCKETOUTPUT_H
#define SOCKETOUTPUT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>

class DummyDataSource : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;
public:
    bool keepRunning;
signals:
    void send(char deviceID, char tag, const QByteArray& payload);
};

class SocketOutput : public QObject
{
    Q_OBJECT

private:
    explicit SocketOutput(QObject *parent = 0);
    QTcpSocket *m_socket;
    QTcpServer *m_server;
    DummyDataSource *m_dummyThread;

public:
    static SocketOutput* instance();

public slots:
    void connected();
    void sendData(char deviceID, char tag, const QByteArray& payload);
    void toogleDummy();

private slots:
    void startDummy();
    void stopDummy();
};

#endif // SOCKETOUTPUT_H
