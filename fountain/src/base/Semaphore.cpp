/*
 * Semaphore.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Mopple
 */

#include <src/base/Semaphore.h>

namespace base {
	
Semaphore::Semaphore(unsigned long initial_count)
	: m_count(initial_count)
{}

void Semaphore::notify() {
	std::unique_lock<decltype(m_mutex_)> lock(m_mutex_);
	++m_count;
	m_condvar.notify_one();
}

void Semaphore::wait(unsigned int timeout) {
	std::unique_lock<decltype(m_mutex_)> lock(m_mutex_);
	while(!m_count) // Handle spurious wake-ups.
		m_condvar.wait(lock);
	--m_count;
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
