//
// Created by jason on 5/11/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"
#include "../src/IoMessage.h"

using namespace ::testing;
using namespace std;

class ConcurrentQueueInstance : public testing::Test {
};

TEST_F(ConcurrentQueueInstance, InitialQueueIsEmpty)
{
    ConcurrentQueue<int> concurrentQueue;
    concurrentQueue.empty();
}

TEST_F(ConcurrentQueueInstance, PushAndTryPop)
{
    ConcurrentQueue<int> concurrentQueue;
    int pushed = 1;
    concurrentQueue.push(pushed);
    int popped;
    concurrentQueue.try_pop(popped);
    ASSERT_THAT(1, popped);
}

TEST_F(ConcurrentQueueInstance, PushAndWaitAndPop)
{
    ConcurrentQueue<int> concurrentQueue;
    int pushed = 1;
    concurrentQueue.push(pushed);
    int popped = concurrentQueue.wait_and_pop();
    ASSERT_THAT(1, popped);
}

TEST_F(ConcurrentQueueInstance, AssignQueueToAnotherQueue)
{
    ConcurrentQueue<int> concurrentQueue;
    concurrentQueue.push(1);
    ConcurrentQueue<int> otherQueue = concurrentQueue;
    int popped = otherQueue.wait_and_pop();
    ASSERT_THAT(1, popped);
}

TEST_F(ConcurrentQueueInstance, PushAndPopSharedPtrInt) {
    ConcurrentQueue<int> concurrentQueue;
    int pushed = 1;
    concurrentQueue.push(pushed);
    int popped;
    concurrentQueue.try_pop(popped);
    ASSERT_EQ(1, popped);
}


TEST_F(ConcurrentQueueInstance, PushAndPopSharedPtrIoMessage) {
    ConcurrentQueue<IoMessage> concurrentQueue;
    IoMessage pushed(std::string("message"), 0, "up");
    concurrentQueue.push(pushed);
    IoMessage popped;
    concurrentQueue.try_pop(popped);
    ASSERT_EQ(std::string("message"), popped.getMessage());
}
