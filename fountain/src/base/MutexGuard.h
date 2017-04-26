/*
 * MutexGuard.h
 *
 *  Created on: 13.03.2017
 *      Author: Mopple
 */

#ifndef SRC_BASE_MUTEXGUARD_H_
#define SRC_BASE_MUTEXGUARD_H_

#include <mutex>

namespace base {

class MutexGuard {
public:
	MutexGuard(std::mutex& mutex);
	virtual ~MutexGuard();

private:
	std::mutex& m_mutex;
};

} /* namespace base */

#endif /* SRC_BASE_MUTEXGUARD_H_ */
