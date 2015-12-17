//
// Created by jason on 5/11/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/ConcurrentQueue.h"
#include "../src/Random.h"
#include "../src/common.h"

using namespace ::testing;
using namespace std;

TEST(ConcurrentQueueTest, InitialQueueIsEmpty)
{
    ConcurrentQueue<io_msg_ptr> concurrentQueue;
    concurrentQueue.empty();
}

TEST(ConcurrentQueueTest, PushAndTryPop)
{
    ConcurrentQueue<io_msg_ptr> concurrentQueue;
    boost::uuids::uuid uuid = Random::getInstance().getNewUUID();
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"), uuid);
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.try_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, PushAndWaitAndPop)
{
    ConcurrentQueue<io_msg_ptr> concurrentQueue;
    boost::uuids::uuid uuid = Random::getInstance().getNewUUID();
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"), uuid);
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.wait_and_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, AssignQueueToAnotherQueue)
{
    ConcurrentQueue<io_msg_ptr> concurrentQueue;
    boost::uuids::uuid uuid = Random::getInstance().getNewUUID();
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"), uuid);
    concurrentQueue.push(pushed);
    ConcurrentQueue<io_msg_ptr> otherQueue = concurrentQueue;
    auto popped = otherQueue.wait_and_pop();

    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}

TEST(ConcurrentQueueTest, PushAndPopSharedPtrIoMessage) {
    ConcurrentQueue<io_msg_ptr> concurrentQueue;
    boost::uuids::uuid uuid = Random::getInstance().getNewUUID();
    std::shared_ptr<IoMessage> pushed = std::make_shared<IoMessage>(std::string("message"), 0, std::string("up"), uuid);
    concurrentQueue.push(pushed);
    auto popped = concurrentQueue.try_pop();
    ASSERT_THAT(popped->getCommand(), Eq(std::string("message")));
}
