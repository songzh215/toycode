#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <memory>
#include <vector>
#include <string>
#include <unistd.h>
#include "stdio.h"


class Observer {
    public:
        Observer(std::string name): _name(name) {
        }
        virtual ~Observer(){
        }
        void register_myself();
        void print();
    private:
        std::string _name;
        std::mutex _mutex;
};

class Observable {
    public:
        Observable() {
        }
        void add(std::shared_ptr<Observer> ob);
        virtual ~Observable(){}
        void printAll();
    private: 
        std::vector<std::shared_ptr<Observer>> observers_ptrs;
        std::mutex _mutex;
};


