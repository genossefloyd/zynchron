#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include "abstractoutput.h"
#include "dummydatasource.h"

class OutputHandler : public AbstractOutput
{
public:
	OutputHandler();

	virtual void sendData(char deviceID, char tag, const ByteArray& payload);

    void toogleDummy();
protected:
    DummyDataSource *m_dummyThread;

    void startDummy();
    void stopDummy();
};

#endif // OUTPUT_HANDLER_H
