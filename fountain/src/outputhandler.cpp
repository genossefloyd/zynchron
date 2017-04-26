#include "outputhandler.h"
#include <iostream>

OutputHandler::OutputHandler()
    : m_dummyThread(NULL)
{
}

static float toFloat(char* array)
{
    float result = *(reinterpret_cast<float*>(array));
    return result;
}

void OutputHandler::sendData(char deviceID, char tag, const ByteArray& payload)
{
	char* data = (char*)payload.data();
	asprintf(&data,"%f,%f,%f",toFloat(data), toFloat(data+4), toFloat(data+8));
	std::cout << "send to " << deviceID << " tag " << tag << " data: " << std::hex << data << std::endl;
	free(data);
}

void OutputHandler::toogleDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();
    else
        startDummy();
}

void OutputHandler::startDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();

    m_dummyThread = new DummyDataSource(this);
    m_dummyThread->start();
}

void OutputHandler::stopDummy()
{
    if(m_dummyThread)
    {
        m_dummyThread->stop();
        m_dummyThread->join();
        delete m_dummyThread;
        m_dummyThread = NULL;
    }
}
