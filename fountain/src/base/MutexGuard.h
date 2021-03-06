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

//Utility Class for mutex class to automatically unlock a given mutex when leaving a context
class MutexGuard {
public:
	MutexGuard(std::mutex& mutex);//locks given mutex
	virtual ~MutexGuard(); //unlocks given mutex

private:
	std::mutex& m_mutex;
};

} /* namespace base */

#endif /* SRC_BASE_MUTEXGUARD_H_ */
