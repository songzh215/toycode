#ifndef TOY_TASK_H
#define TOY_TASK_H

#include <stdio.h>
#define gettid() syscall(SYS_gettid)

namespace toy {
namespace util {

class Task {

    public:
        Task(std::string name) : _task_name(name) {}
        ~Task() {}
        void run() {
            sleep(10);
            printf("task %s executed in thread %ld \n", _task_name.c_str(), gettid());
        }

    private:
        std::string _task_name;
};

}
}
#endif //TOY_TASK_H
