#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include <queue>
#include <condition_variable>    // std::condition_variable
#include "stdio.h"

namespace toy {
namespace util {

template<class T>
class BlockingQueue {
    public:
        BlockingQueue() {}
        virtual ~BlockingQueue() {}
        void put (T& element);
        T take ();
        int size();

    private:
        std::mutex _mutex;
        std::queue<T> _queue;
        std::condition_variable _con;
};
}
}

#endif  //BLOCKING_QUEUE_H
