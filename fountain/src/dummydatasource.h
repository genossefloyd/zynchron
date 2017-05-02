#ifndef DUMMYDATASOURCE_H
#define DUMMYDATASOURCE_H

#include "abstractoutput.h"
#include "base/Thread.h"

class DummyDataSource : public base::Thread
{
public:
	explicit DummyDataSource(AbstractOutput* output);
	virtual ~DummyDataSource();

	void stop();

private:
    bool m_keepRunning;
    AbstractOutput* m_output;

    void execute();
};

#endif // DUMMYDATASOURCE_H
