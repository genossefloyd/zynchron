/*
 * MutexGuard.cpp
 *
 *  Created on: 13.03.2017
 *      Author: Mopple
 */

#include <src/base/MutexGuard.h>

namespace base {

MutexGuard::MutexGuard(std::mutex& mutex)
: m_mutex(mutex)
{
	m_mutex.lock();
}

MutexGuard::~MutexGuard()
{
	m_mutex.unlock();
}

} /* namespace base */
