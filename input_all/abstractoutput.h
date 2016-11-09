#ifndef ABSTRACTOUTPUT_H
#define ABSTRACTOUTPUT_H

#include <QObject>
#include "dummydatasource.h"

class AbstractOutput : public QObject
{
    Q_OBJECT
public:
    explicit AbstractOutput(QObject *parent = 0);

public slots:
    void toogleDummy();

protected:
    DummyDataSource *m_dummyThread;

private slots:
    void startDummy();
    void stopDummy();
};

#endif // ABSTRACTOUTPUT_H
