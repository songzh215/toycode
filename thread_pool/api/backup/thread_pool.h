#ifndef THREAD_POOL
#define THREAD_POOL

#include "blocking_queue_bounded.h"
#include "task.h"



namespace toy {
namespace util {

class thread_pool {
    public:
        thread_pool(int thread_num, int task_num);
        // append task T to thread_pool
        bool execute(Task& t);
        void start();
        void stop();
        ~thread_pool();

    private:

        int _thread_num;
        int _task_num;
        toy::util::BlockingQueueBounded<Task> _task_queue;
        bool _running;
        std::vector<std::shared_ptr<std::thread>> _workers;
};
}
}

#endif  //THREAD_POOL
