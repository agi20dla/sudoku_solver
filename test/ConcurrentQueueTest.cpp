//
// Created by jason on 5/11/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"

using namespace ::testing;
using namespace std;

TEST(ConcurrentQueueTest, InitialQueueIsEmpty)
{
    ConcurrentQueue concurrentQueue;
    concurrentQueue.empty();
}

TEST(ConcurrentQueueTest, PushAndTryPop)
{
    ConcurrentQueue concurrentQueue;
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"));
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.try_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, PushAndWaitAndPop)
{
    ConcurrentQueue concurrentQueue;
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"));
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.wait_and_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, AssignQueueToAnotherQueue)
{
    ConcurrentQueue concurrentQueue;
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"));
    concurrentQueue.push(pushed);
    ConcurrentQueue otherQueue = concurrentQueue;
    auto popped = otherQueue.wait_and_pop();

    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, PushAndPopSharedPtrIoMessage) {
    ConcurrentQueue concurrentQueue;
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"));
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.try_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}
