/*
 * Semaphore.h
 *
 *  Created on: 13.03.2017
 *      Author: Mopple
 */

#ifndef SRC_BASE_SEMAPHORE_H_
#define SRC_BASE_SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

namespace base {

class Semaphore
{
private:
    std::mutex m_mutex_;
    std::condition_variable m_condvar;
    unsigned long m_count;

public:

	Semaphore(unsigned long initial_count = 0);  // Initialized as locked.
    void notify();
    void wait(unsigned int timeout = 0);
    bool try_wait();
};

} /* namespace base */

#endif /* SRC_BASE_SEMAPHORE_H_ */
