//
// Created by jason on 5/11/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"

using namespace ::testing;
using namespace std;

class ConcurrentQueueInstance : public testing::Test {
public:
    ConcurrentQueue<int> concurrentQueue;
};

TEST_F(ConcurrentQueueInstance, InitialQueueIsEmpty)
{
    concurrentQueue.empty();
}

TEST_F(ConcurrentQueueInstance, PushAndTryPop)
{
    int pushed = 1;
    concurrentQueue.push(pushed);
    int popped;
    concurrentQueue.try_pop(popped);
    ASSERT_THAT(popped, 1);
}

TEST_F(ConcurrentQueueInstance, PushAndWaitAndPop)
{
    int pushed = 1;
    concurrentQueue.push(pushed);
    int popped;
    concurrentQueue.wait_and_pop(popped);
    ASSERT_THAT(popped, 1);
}

TEST_F(ConcurrentQueueInstance, AssignQueueToAnotherQueue)
{
    concurrentQueue.push(1);
    ConcurrentQueue<int> otherQueue = concurrentQueue;
    int popped;
    otherQueue.wait_and_pop(popped);
    ASSERT_THAT(popped, 1);
}
