#include "thread_pool.h"
#include "task.h"



namespace toy {
namespace util {
    template<class T>
    static void thread_run(toy::util::BlockingQueueBounded<T> * queue) {
        printf("thread %ld start... \n", gettid());
        while (true) {
            T t = queue->take();
            t.run(); 
        }
    }

    template<class T>
    thread_pool<T>::thread_pool(int thread_num, int task_num): _thread_num(thread_num), _task_num(task_num),
        _task_queue(task_num) , _running(true) {
    }
    
    template<class T>
    void thread_pool<T>::start() {
        for (int i = 0 ; i < _thread_num ; i++) {
            printf("start one thread...\n");
            std::thread *thread = new std::thread(&thread_run<T>, &_task_queue);
            _workers.push_back(std::move(std::shared_ptr<std::thread>(thread)));
        }
    }

    template<class T>
    bool thread_pool<T>::execute(T& t) {
        if (!_running) {
            return false;
        }
        _task_queue.put(t);
        return true;
    }

    template<class T>
    thread_pool<T>::~thread_pool() {
    }

    template<class T>
    void thread_pool<T>::wait() {
        _running = false;
        for (int i = 0 ; i < _thread_num ; i++) {
            _workers[i]->join();
        }
    }
}
}

