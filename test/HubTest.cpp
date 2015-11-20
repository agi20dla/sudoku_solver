//
// Created by jason on 5/22/15.
//


#include <string>
#include <thread>
#include "gmock/gmock.h"

#include "../src/IoMessage.h"
#include "../src/IoPort.h"
#include "../src/CellHub.h"

using namespace std;

TEST(HubTest, HubPullsMessageFromQueueAndGivesToIoPort)
{
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    std::shared_ptr<Hub> hub(make_shared<CellHub>());

    std::shared_ptr<IoPort> ioPort = make_shared<IoPort>(hub_ptr(hub), msgsReceived, "g");
    hub->addIoPort(ioPort);

    msg_ptr ioMessage = make_shared<IoMessage>("message");
    hub->push(ioMessage);

    hub->run();

    ASSERT_TRUE(hub->numMessagesOnQueue() == 0);
    ASSERT_EQ(ioPort->getNumMessagesRecieved(),1);
    ASSERT_EQ(ioPort->getNumMessagesForwarded(), 0);
    ASSERT_EQ(ioPort->getNumMessagesSent(), 0);
}


