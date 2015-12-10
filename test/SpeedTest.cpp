//
// Created by jason on 12/10/15.
//


#include <string>
#include <chrono>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"
#include "../src/IoMessage.h"
#include "../src/Hub.h"

using namespace ::testing;
using namespace std;


template<typename TimeT = std::chrono::milliseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args &&... args) {
        auto start = std::chrono::system_clock::now();

        // Now call the function with all the parameters you need.
        func(std::forward<Args>(args)...);

        auto duration = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);

        return duration.count();
    }
};

template<typename Q>
void queueLoad(ConcurrentQueue<shared_ptr<Q>> queue, long numElements) {
    for (int idx = 0; idx < numElements; idx++) {
        queue.push(make_shared<Q>());
    }
}

// Best time = ~36ms
TEST(SpeedTest, ConcurrentQueueSPInt10K) {
    ConcurrentQueue<shared_ptr<int>> queue;
    long numElements = 10000;

    std::cout << endl << "Time: " << measure<>::execution([&]() {
        queueLoad(queue, numElements);
    }) << endl;
}

// Best time = ~44ms
TEST(SpeedTest, ConcurrentQueueIoMessage10K) {
    ConcurrentQueue<shared_ptr<IoMessage>> queue;
    long numElements = 10000;

    std::cout << endl << "Time: " << measure<>::execution([&]() {
        queueLoad(queue, numElements);
    }) << endl;
}

// Best time = ~121ms
TEST(SpeedTest, ConcurrentQueueHub10K) {
    ConcurrentQueue<shared_ptr<Hub>> queue;
    long numElements = 10000;

    std::cout << endl << "Time: " << measure<>::execution([&]() {
        queueLoad(queue, numElements);
    }) << endl;
}
