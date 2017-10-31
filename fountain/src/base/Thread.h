/*
 * Thread.h
 *
 *  Created on: 21.02.2017
 *      Author: Mopple
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <thread>

namespace base {

//Abstract base class for simple thread implementation using c++11 std::thread class
class Thread {
public:
	Thread();
	virtual ~Thread();

	void start(); //start thread
	void join(); //wait for thread completion and free its resources

	bool isRunning(); //returns true if thread is still active and running

	static void sleep(int s); //sleep seconds
	static void sleepMs(int ms); //sleep milli seconds
private:
	std::thread m_thread;
	bool m_isRunning;

	static void executer(Thread*);

protected:
	//overwrite this method with implementation for this thread
	virtual void execute() = 0;
};

} /* namespace base */

#endif /* SRC_BASE_THREAD_H_ */
