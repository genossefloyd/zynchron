#ifndef DUMMYDATASOURCE_H
#define DUMMYDATASOURCE_H

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

#endif // DUMMYDATASOURCE_H
