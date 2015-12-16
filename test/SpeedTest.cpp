//
// Created by jason on 12/10/15.
//


#include <string>
#include <chrono>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"
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

void queueLoad(ConcurrentQueue<io_msg_ptr> queue, long numElements) {
    for (int idx = 0; idx < numElements; idx++) {
        queue.push(make_shared<IoMessage>());
    }
}

// Best time = ~44ms
TEST(SpeedTest, ConcurrentQueueIoMessage50K) {
    ConcurrentQueue<io_msg_ptr> queue;
    long numElements = 50000;

    std::cout << endl << "Time: " << measure<>::execution([&]() {
        queueLoad(queue, numElements);
    }) << endl;
}
