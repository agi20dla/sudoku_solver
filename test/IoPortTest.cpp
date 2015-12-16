//
// Created by jason on 5/16/15.
//


#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "../src/IoPort.h"
#include "../src/CellHub.h"

using namespace ::testing;
using namespace std;


TEST(IoPortTest, IoPortForwardsAMessageToAQueue)
{
    cell_hub_ptr hub = make_shared<CellHub>();
    msg_map_ptr msgsReceived = make_shared<msg_map>();

    IoPort ioPort;
    std::string direction("g");
    ioPort.init(hub, msgsReceived, direction);
//    ioPort.setHub(hub);
//    ioPort.addRcvdMsgMap(msgsReceived);
//    ioPort.setDirection(string("g"));

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("t"));
    ioPort.fwdToQueue(ioMessage);

    auto poppedMessage = hub->tryPop();

    ASSERT_THAT(poppedMessage->getCommand(), Eq(std::string("message")));
}

TEST(IoPortTest, IoPortSendsAMessageToAnotherIoPort)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    msg_map_ptr msgsReceived1 = make_shared<msg_map>();
    io_ptr port1 = make_shared<IoPort>();
    port1->init(hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    msg_map_ptr msgsReceived2 = make_shared<msg_map>();
    io_ptr port2 = make_shared<IoPort>();
    port2->init(hub_ptr(cellHub2), msgsReceived2, "g");

    port1->connect(port2);
    port2->connect(port1);

    auto ioMessage1 = std::make_shared<IoMessage>(std::string("message"), 0, "g");
    port1->sendToExt(ioMessage1);

    auto poppedMessage = cellHub2->tryPop();
    ASSERT_THAT(poppedMessage->getCommand(), Eq(std::string("message")));
}

TEST(IoPortTest, IoPortExceptsOnMultipleConnects)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    msg_map_ptr msgsReceived1 = make_shared<msg_map>();
    io_ptr port1 = make_shared<IoPort>();
    port1->init(cellHub1, msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    msg_map_ptr msgsReceived2 = make_shared<msg_map>();
    io_ptr port2 = make_shared<IoPort>();
    port2->init(cellHub2, msgsReceived2, "g");

    cell_hub_ptr cellHub3 = make_shared<CellHub>();
    msg_map_ptr msgsReceived3 = make_shared<msg_map>();
    io_ptr port3 = make_shared<IoPort>();
    port3->init(cellHub3, msgsReceived3, "g");

    port1->connect(port2);
    EXPECT_ANY_THROW(port1->connect(port3));
}

TEST(IoPortTest, IoPortAddsUuidToForwardedMessage)
{
    cell_hub_ptr cellHub = make_shared<CellHub>();
    msg_map_ptr msgsReceived1 = make_shared<msg_map>();

    IoPort ioPort;
    ioPort.init(cellHub, msgsReceived1, "g");

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("g"));
    ioPort.fwdToQueue(ioMessage);

    auto poppedMessage = cellHub->tryPop();

    boost::uuids::uuid rcvPortUuid = poppedMessage->getForwardingPortUUID();
    boost::uuids::uuid ioPortUuid = ioPort.getUuid();

    ASSERT_EQ(rcvPortUuid, ioPortUuid);

}
