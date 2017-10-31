#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include "abstractoutput.h"
#include "dummydatasource.h"

//implementation/extension of interface class "AbstractOutput"
//can send dummy test data via the "sendData" interface
class OutputHandler : public AbstractOutput
{
public:
	OutputHandler();
	~OutputHandler();

	//simple test implementation which prints data on console output
	virtual void sendData(char deviceID, char tag, const ByteArray& payload);

	//toggle (turn on/off) dummy data generation
    void toogleDummy();
protected:
    DummyDataSource *m_dummyThread;

    //creates a new thread which will generate and issue dummy data
    void startDummy();
    //stops dummy data generation, stops thread and joins it
    void stopDummy();
};

#endif // OUTPUT_HANDLER_H
