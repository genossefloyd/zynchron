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
//implementation of a simple semaphore using c++11 std library classes
class Semaphore
{
private:
    std::mutex m_mutex_;
    std::condition_variable m_condvar;
    unsigned long m_count;

public:

	Semaphore(unsigned long initial_count = 0);  // Initialized as locked.
    void notify(); //increment count by one and notify one waiting thread
    //decrement count and wait until count reaches value > 0 or timeout occurs
    //use timeout of 0 to wait infinitely
    //returns true if semaphore was signaled before timeout
    bool wait(unsigned int timeout = 0);

    //check if semaphore is available without blocking
    //returns true if semaphore is available, which is decrementing count
    bool try_wait();
};

} /* namespace base */

#endif /* SRC_BASE_SEMAPHORE_H_ */
