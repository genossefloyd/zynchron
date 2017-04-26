/*
 * Thread.cpp
 *
 *  Created on: 21.02.2017
 *      Author: Mopple
 */

#include <src/base/Thread.h>

using namespace base;

Thread::Thread()
	: m_isRunning(false)
{}

Thread::~Thread()
{
	join();
}

void Thread::start()
{
	m_thread = std::thread(executer, this);
}
void Thread::join()
{
	if(m_thread.joinable())
	{
		m_thread.join();
	}
}

bool Thread::isRunning()
{
	return m_isRunning;
}

void Thread::executer(Thread* t)
{
	if(t)
	{
		t->m_isRunning = true;
		t->execute();
		t->m_isRunning = false;
	}
}

void Thread::sleep(int s)
{
	std::this_thread::sleep_for (std::chrono::seconds(s));
}

void Thread::sleepMs(int ms)
{
	std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}
