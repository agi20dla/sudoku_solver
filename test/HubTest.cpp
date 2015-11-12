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
    shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    shared_ptr<Hub> hub(make_shared<CellHub>());

    shared_ptr<IoPort> ioPort = make_shared<IoPort>(hub, msgsReceived, "g");
    hub->addIoPort(ioPort);

    IoMessage ioMessage("message");
    hub->push(ioMessage);

    hub->run();

    ASSERT_TRUE(hub->numMessagesOnQueue() == 0);
    ASSERT_EQ(ioPort->getNumMessagesRecieved(),1);
    ASSERT_EQ(ioPort->getNumMessagesForwarded(), 0);
    ASSERT_EQ(ioPort->getNumMessagesSent(), 0);
}


