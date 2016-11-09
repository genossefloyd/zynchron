#include "dummydatasource.h"
#include "messages.h"

#include <qmath.h>

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
