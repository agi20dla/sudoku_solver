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
    std::shared_ptr<CellHub> hub(make_shared<CellHub>());

    std::shared_ptr<IoPort> ioPort = make_shared<IoPort>();
    std::string direction("g");
    ioPort->init(hub, direction);
    hub->addIoPort(ioPort);

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("g"), hub->getUUID());
    hub->push(ioMessage);
    ASSERT_TRUE(hub->getNumMsgsOnQueue() == 1);

    hub->run();

    ASSERT_TRUE(hub->getNumMsgsOnQueue() == 0);
}


