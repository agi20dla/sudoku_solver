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
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    std::shared_ptr<CellHub> hub(make_shared<CellHub>());

    std::shared_ptr<IoPort> ioPort = make_shared<IoPort>(cell_hub_ptr(hub), msgsReceived, "g");
    hub->addIoPort(ioPort);

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("g"));
    hub->push(ioMessage);

    hub->run();

    ASSERT_TRUE(hub->getNumMsgsOnQueue() == 0);
    ASSERT_EQ(ioPort->getNumMessagesRecieved(),1);
    ASSERT_EQ(ioPort->getNumMsgsForwardedToHub(), 0);
    ASSERT_EQ(ioPort->getNumMessagesSent(), 0);
}


