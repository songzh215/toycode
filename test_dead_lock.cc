#include "test_dead_lock.h"
#include <sys/types.h>


static Observable g_obs;


void Observer::register_myself() {
    std::lock_guard<std::mutex> lock(_mutex);
    printf("get observer lock register %d\n", std::this_thread::get_id());
    sleep(3);
    g_obs.add(std::shared_ptr<Observer>(this));
}
void Observer::print() {
    std::lock_guard<std::mutex> lock(_mutex);
    printf("get observer lock print %d\n",std::this_thread::get_id());
}


void Observable::add(std::shared_ptr<Observer> ob) {
    std::lock_guard<std::mutex> lock(_mutex);
    printf("get observable lock add thread id %d\n", std::this_thread::get_id());
    sleep(3);
    observers_ptrs.push_back(ob);
}

void Observable::printAll() {
    std::lock_guard<std::mutex> lock(_mutex);
    printf("get observable printall lock thread id %d\n", std::this_thread::get_id());
    sleep(5);
    for (auto& ptr : observers_ptrs) {
        ptr->print();
    }
}

void foo() {
    std::string name1("ob1");
    Observer* ob1 = new Observer(name1);
    ob1->register_myself();
    delete ob1;
}

int main() {
    std::thread thread1(&foo);
    sleep(3);
    g_obs.printAll();
    if (thread1.joinable()) {
        thread1.join();
    }
    return 0;
}
