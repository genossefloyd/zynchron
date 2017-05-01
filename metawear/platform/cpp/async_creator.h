#pragma once

#include "platform/cpp/concurrent_queue.h"
#include "platform/cpp/task.h"

#include <functional>

struct AsyncCreator {
    virtual ~AsyncCreator();

    ConcurrentQueue<std::function<void (void)>> pending_fns;
    Task* timeout;

    void create_next(bool force);    
};
