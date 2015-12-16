//
// Created by jason on 5/16/15.
//


#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "../src/IoPort.h"
#include "../src/CellHub.h"
#include "../src/Random.h"

using namespace ::testing;
using namespace std;


TEST(IoPortTest, IoPortForwardsAMessageToAQueue)
{
    cell_hub_ptr hub = make_shared<CellHub>();

    IoPort ioPort;
    std::string direction("g");
    ioPort.init(hub, direction);
    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("t"),
                                                 Random::getInstance().getNewUUID());
    ioPort.fwdToQueue(ioMessage);

    auto poppedMessage = hub->tryPop();

    ASSERT_THAT(poppedMessage->getCommand(), Eq(std::string("message")));
}

TEST(IoPortTest, IoPortSendsAMessageToAnotherIoPort)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    io_ptr port1 = make_shared<IoPort>();
    port1->init(hub_ptr(cellHub1), std::string("g"));

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    io_ptr port2 = make_shared<IoPort>();
    port2->init(hub_ptr(cellHub2), std::string("g"));

    port1->connect(port2);
    port2->connect(port1);

    auto ioMessage1 = std::make_shared<IoMessage>(std::string("message"), 0, std::string("g"), cellHub1->getUUID());
    port1->sendToExt(ioMessage1);

    auto poppedMessage = cellHub2->tryPop();
    ASSERT_THAT(poppedMessage->getCommand(), Eq(std::string("message")));
}

TEST(IoPortTest, IoPortExceptsOnMultipleConnects)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    io_ptr port1 = make_shared<IoPort>();
    port1->init(cellHub1, std::string("g"));

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    io_ptr port2 = make_shared<IoPort>();
    port2->init(cellHub2, std::string("g"));

    cell_hub_ptr cellHub3 = make_shared<CellHub>();
    io_ptr port3 = make_shared<IoPort>();
    port3->init(cellHub3, std::string("g"));

    port1->connect(port2);
    EXPECT_ANY_THROW(port1->connect(port3));
}
