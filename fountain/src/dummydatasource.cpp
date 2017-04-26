#include "dummydatasource.h"
#include "messages.h"

#include <cmath>
#include <stdint.h>
#include <string.h>

DummyDataSource::DummyDataSource(AbstractOutput* output)
: m_keepRunning(false)
, m_output(output)
{
}

DummyDataSource::~DummyDataSource()
{}

void DummyDataSource::stop()
{
	m_keepRunning = false;
}

void DummyDataSource::execute()
{
	m_keepRunning = true;

    float x = 0.0f, y = 1.57f, z = 3.14f;
    int len = sizeof(float);
    ByteArray dummyData;

    while(m_keepRunning)
    {
    	dummyData.resize(len*3);

        float value = std::sin(x);
        memcpy(dummyData.data(),(char*)&value,len);
        value = std::sin(y);
        memcpy(dummyData.data()+len,(char*)&value,len);
        value = std::sin(z);
        memcpy(dummyData.data()+(2*len),(char*)&value,len);

        m_output->sendData(0xa9, msg::ACCELEROMETER, dummyData);
        dummyData.clear();
        x+=0.1f;
        y+=0.1f;
        z+=0.1f;
        sleepMs(250);
    }
}
