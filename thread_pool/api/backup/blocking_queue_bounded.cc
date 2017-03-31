#include "blocking_queue_bounded.h"
#include <signal.h>

// 未考虑异常的处理等，仅仅实现基本的bounded blocking queue功能.
namespace toy {
namespace util {

template <class T>
void BlockingQueueBounded<T>::put(T& element) {
    std::unique_lock <std::mutex> lock(_mutex);
    while (_queue.size() >= _capacity) {
        _full_con.wait(lock);
    }
    _queue.push(element);
    printf("push a element");
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

class Foo {
    public:
        Foo(std::string& name) {
            _name = name;
        }
        void print() {
            printf("consume %s \n", _name.c_str());
        }
    private:
        std::string _name;
};

void produce(toy::util::BlockingQueueBounded<Foo>* que) {
    printf("producer thread %ld begins \n", gettid());
    std::string str1("foo1");
    Foo f1(str1);
    que->put(f1);
    printf("producer thread %ld produce %s \n", gettid(), str1.c_str());
    std::string str2("foo2");
    Foo f2(str2);
    que->put(f2);
    printf("producer thread %ld produce %s \n", gettid(), str2.c_str());
    // 应该会满, block住
    std::string str3("foo3");
    Foo f3(str3);
    que->put(f3);
    printf("producer thread %ld produce %s \n", gettid(), str3.c_str());
    std::string str4("foo4");
    Foo f4(str4);
    que->put(f4);
    printf("producer thread %ld produce %s \n", gettid(), str4.c_str());
    printf("producer thread %ld ends \n", gettid());
}

//toy::util::BlockingQueue<Foo>& que
void consume(toy::util::BlockingQueueBounded<Foo>* que) {
    while (1) {
        sleep(5);
        que->take().print();
    }
}

void sigterm(int signo) {
    printf("signal %d received.\n", signo);
}

int main() {
    signal(SIGTERM, sigterm);
    toy::util::BlockingQueueBounded<Foo> que(3);
    std::thread producer(&produce, &que);
    std::thread producer1(&produce, &que);
    std::thread producer2(&produce, &que);
    std::thread consumer(&consume, &que);
//    std::thread consumer1(&consume, &que);
//    std::thread consumer2(&consume, &que);
//    std::thread consumer3(&consume, &que);
//    std::thread consumer4(&consume, &que);
    if (producer.joinable()) {
        producer.join();
    }
    if (producer1.joinable()) {
        producer1.join();
    }
    if (producer2.joinable()) {
        producer2.join();
    }
    if (consumer.joinable()) {
        consumer.join();
    }
/*    if (consumer1.joinable()) {
        consumer1.join();
    }
    if (consumer2.joinable()) {
        consumer2.join();
    }
    if (consumer3.joinable()) {
        consumer3.join();
    }
    if (consumer4.joinable()) {
        consumer4.join();
    } */
}
