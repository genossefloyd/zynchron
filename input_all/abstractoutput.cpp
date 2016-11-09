#include "abstractoutput.h"

AbstractOutput::AbstractOutput(QObject *parent)
    : QObject(parent)
    , m_dummyThread(NULL)
{
}

void AbstractOutput::toogleDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();
    else
        startDummy();
}

void AbstractOutput::startDummy()
{
    if(m_dummyThread && m_dummyThread->isRunning())
        stopDummy();

    m_dummyThread = new DummyDataSource();

    connect(m_dummyThread, SIGNAL(finished()), this, SLOT(stopDummy()));
    connect(m_dummyThread, SIGNAL(send(char,char,QByteArray)), this, SLOT(sendData(char,char,QByteArray)));
    m_dummyThread->start();
}

void AbstractOutput::stopDummy()
{
    if(m_dummyThread)
    {
        m_dummyThread->keepRunning = false;
        m_dummyThread->wait();
        delete m_dummyThread;
        m_dummyThread = NULL;
    }
}
