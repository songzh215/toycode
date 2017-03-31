#include "blocking_queue.h"
#include <signal.h>

// 未考虑异常的处理等，仅仅实现基本的unbounded blocking queue功能.
namespace toy {
namespace util {
template <class T>
void BlockingQueue<T>::put(T& element) {
    std::unique_lock <std::mutex> lock(_mutex);
    _queue.push(element);
    //触发虚假唤醒
    _con.notify_all();
    //_con.notify_one();
}

template <class T>
T BlockingQueue<T>::take() {
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
    return ele;
}
    
template <class T>
int BlockingQueue<T>::size() {
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
            printf("print %s \n", _name.c_str());
        }
    private:
        std::string _name;
};

void produce(toy::util::BlockingQueue<Foo>* que) {
    std::string str1("foo1");
    Foo f1(str1);
    que->put(f1);
    sleep(3);
    std::string str2("foo2");
    Foo f2(str2);
    que->put(f2);
    printf("producer do something\n");
}

//toy::util::BlockingQueue<Foo>& que
void consume(toy::util::BlockingQueue<Foo>* que) {
    while (1) {
        printf("consumer waits to take\n");
        que->take().print();
    }
}

void sigterm(int signo) {
    printf("signal %d received.\n", signo);
}

int main() {
    signal(SIGTERM, sigterm);
    toy::util::BlockingQueue<Foo> que;
    std::thread producer(&produce, &que);
    std::thread consumer(&consume, &que);
    std::thread consumer1(&consume, &que);
    std::thread consumer2(&consume, &que);
    std::thread consumer3(&consume, &que);
    std::thread consumer4(&consume, &que);
    std::thread consumer5(&consume, &que);
    std::thread consumer6(&consume, &que);
    std::thread consumer7(&consume, &que);
    std::thread consumer8(&consume, &que);
    std::thread consumer9(&consume, &que);
    std::thread consumer10(&consume, &que);
    if (producer.joinable()) {
        producer.join();
    }
    if (consumer.joinable()) {
        consumer.join();
    }
    if (consumer1.joinable()) {
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
    }
    if (consumer5.joinable()) {
        consumer5.join();
    }
    if (consumer6.joinable()) {
        consumer6.join();
    }
    if (consumer7.joinable()) {
        consumer7.join();
    }
    if (consumer8.joinable()) {
        consumer8.join();
    }
    if (consumer9.joinable()) {
        consumer9.join();
    }
    if (consumer10.joinable()) {
        consumer10.join();
    }
    printf("main ends\n");
}
