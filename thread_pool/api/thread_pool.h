#ifndef THREAD_POOL
#define THREAD_POOL

#include "blocking_queue_bounded.h"



namespace toy {
namespace util {

template<class T>
class thread_pool {
    public:
        thread_pool(int thread_num, int task_num);
        // append task T to thread_pool
        bool execute(T& t);
        void start();
        void wait();
        ~thread_pool();

    private:

        int _thread_num;
        int _task_num;
        toy::util::BlockingQueueBounded<T> _task_queue;
        bool _running;
        std::vector<std::shared_ptr<std::thread>> _workers;
};

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
            thread->detach();
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

#endif  //THREAD_POOL
