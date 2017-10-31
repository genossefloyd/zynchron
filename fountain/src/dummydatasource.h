#ifndef DUMMYDATASOURCE_H
#define DUMMYDATASOURCE_H

#include "abstractoutput.h"
#include "base/Thread.h"

//test data source sending generated test data to the given output object
//derived from Thread class to run in background in a separate thread
//test data is a tuple of 3 coordinates (x,y,z)
//each parameter is the result function cos(i) with i incremented by 0.1 every 250ms
//each parameter starts with a different offset ( offset x = 0, offset y = pi/2, offset z = pi)
class DummyDataSource : public base::Thread
{
public:

	explicit DummyDataSource(AbstractOutput* output);
	virtual ~DummyDataSource();

	void stop(); //stop thread execution

private:
    bool m_keepRunning;
    AbstractOutput* m_output;

    void execute();
};

#endif // DUMMYDATASOURCE_H
