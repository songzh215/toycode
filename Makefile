g++ -std=c++11 test_dead_lock.cc -o hello -pthread


g++ -std=c++11 blocking_queue.cc -o hello -pthread


g++ test_namespace.cc -I /usr/local/include/boost/ -L /usr/local/lib/ -lboost_filesystem -lboost_system
