#ifndef SOCKETOUTPUT_H
#define SOCKETOUTPUT_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>

class DummyDataSource : public QThread
{
    void run() Q_DECL_OVERRIDE;
public:
    bool keepRunning;
};

class SocketOutput : public QObject
{
    Q_OBJECT

private:
    explicit SocketOutput(QObject *parent = 0);
    QUdpSocket *m_socket;

public:
    static SocketOutput* instance();
    bool sendData(char deviceID, char tag, const QByteArray& payload);

signals:

public slots:
    void toogleDummy();
private slots:
    void startDummy();
    void stopDummy();
private:
    DummyDataSource *m_dummyThread;
};

#endif // SOCKETOUTPUT_H
