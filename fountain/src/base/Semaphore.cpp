/*
 * Semaphore.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Mopple
 */

#include <src/base/Semaphore.h>

#include <stdlib.h>
#include <chrono>

namespace base {
	
Semaphore::Semaphore(unsigned long initial_count)
	: m_count(initial_count)
{}

void Semaphore::notify() {
	std::unique_lock<decltype(m_mutex_)> lock(m_mutex_);
	++m_count;
	printf("notify %lu\n", m_count);
	m_condvar.notify_one();
}

bool Semaphore::wait(unsigned int timeout) {
	std::unique_lock<decltype(m_mutex_)> lock(m_mutex_);
	printf("wait start\n");

	bool success = m_condvar.wait_for(lock, std::chrono::milliseconds(timeout), [this]() { return m_count > 0; });
	if(success)
	{
		--m_count;
		printf("wait end %lu\n", m_count);
	}
	else
		printf("wait timeout\n");

	return success;
}

bool Semaphore::try_wait() {
	std::unique_lock<decltype(m_mutex_)> lock(m_mutex_);
	if(m_count) {
		--m_count;
		return true;
	}
	return false;
}

} /* namespace base */
