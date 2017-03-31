#include "api/thread_pool.h"
#include "task.h"
#include <sstream>


int main() {

    toy::util::thread_pool<toy::util::Task> pool(10, 20);
    pool.start();

    for (int i = 0 ; i < 100 ; i++) {
        std::stringstream ss;
        ss << "t" << i;
        toy::util::Task task(ss.str());
        pool.execute(task);
    }
    printf("main ends \n");
    pool.wait();
}
