//
// Created by jason on 5/23/15.
//

#include <string>
#include <thread>
#include "gmock/gmock.h"

#include "../src/CellHub.h"

using namespace testing;

/**
 * Set up a hub with a message queue and an IO port
 * Set up another IO port with a message queue, but not managed by a hub
 * Connect port 1 to port 2 (port 1 can send to port 2)
 * Create a message and put it on the queue
 *
 * When the hub is run, the message should show up on port 2's queue
 */
TEST(Integration, MessageTravelsFromPort1QueueToHubToPort2)
{
    std::shared_ptr<CellHub> hub1 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    std::shared_ptr<IoPort> port1 = make_shared<IoPort>(cell_hub_ptr(hub1), msgsReceived1, "g");

    hub1->addIoPort(port1);

    std::shared_ptr<CellHub> hub2 = make_shared<CellHub>();
    std::shared_ptr<ConcurrentQueue<IoMessage>> messageQueue2 = make_shared<ConcurrentQueue<IoMessage>>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived2 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    std::shared_ptr<IoPort> port2 = make_shared<IoPort>(cell_hub_ptr(hub2), msgsReceived2, "g");

    port1->connect(port2);

    msg_ptr ioMessage = make_shared<IoMessage>(string("message"));

    hub1->push(ioMessage);

    hub1->run();

    ASSERT_FALSE(hub2->numMessagesOnQueue() == 0);
}
