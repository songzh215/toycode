#ifndef BLOCKING_QUEUE_BOUNDED_H
#define BLOCKING_QUEUE_BOUNDED_H

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <queue>
#include <condition_variable>    // std::condition_variable
#include "stdio.h"


#define gettid() syscall(SYS_gettid)


namespace toy {
namespace util {

// bounded blocking queue.
template<class T>
class BlockingQueueBounded {
    public:
        BlockingQueueBounded(int max_length): _capacity(max_length) {}
        virtual ~BlockingQueueBounded() {}
        void put(T& element);
        T take();
        int size();

    private:
        int _capacity;
        std::mutex _mutex;
        std::queue<T> _queue;
        // empty condition variable
        std::condition_variable _con;
        // full condition variable
        std::condition_variable _full_con;
};

template <class T>
void BlockingQueueBounded<T>::put(T& element) {
    std::unique_lock <std::mutex> lock(_mutex);
    while (_queue.size() >= _capacity) {
        _full_con.wait(lock);
    }
    _queue.push(element);
    //触发虚假唤醒
    _con.notify_all();
    //_con.notify_one();
}

template <class T>
T BlockingQueueBounded<T>::take() {
    std::unique_lock <std::mutex> lock(_mutex);
    while (_queue.empty()) {
        _con.wait(lock);
    }
    /*if (_queue.empty()) {
        _con.wait(lock);
        printf("hhhhh\n");
    }*/
    T ele = _queue.front();
    _queue.pop();
    if (_queue.size() < _capacity) {
        _full_con.notify_all();
    }
    return ele;
}
    
template <class T>
int BlockingQueueBounded<T>::size() {
    std::unique_lock <std::mutex> lock(_mutex);
    int res = _queue.size();
    lock.unlock();
    return res;
}

}
}

#endif  //BLOCKING_QUEUE_BOUNDED_H
