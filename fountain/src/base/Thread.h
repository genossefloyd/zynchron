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

class Thread {
public:
	Thread();
	virtual ~Thread();

	void start();
	void join();

	bool isRunning();

	static void sleep(int s); //sleep seconds
	static void sleepMs(int ms); //sleep milli seconds
private:
	std::thread m_thread;
	bool m_isRunning;

	static void executer(Thread*);

protected:
	virtual void execute() = 0;
};

} /* namespace base */

#endif /* SRC_BASE_THREAD_H_ */
