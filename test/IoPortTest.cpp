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
    auto hub = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();

    IoPort ioPort(hub, msgsReceived, "g");

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("t"));
    ioPort.fwdToQueue(ioMessage);

    auto poppedMessage = hub->tryPop();

    ASSERT_THAT(poppedMessage->getCommand(), Eq("message"));
}

TEST(IoPortTest, IoPortSendsAMessageToAnotherIoPort)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived1 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    io_ptr port1 = make_shared<IoPort>(cell_hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived2 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    io_ptr port2 = make_shared<IoPort>(cell_hub_ptr(cellHub2), msgsReceived2, "g");

    port1->connect(port2);
    port2->connect(port1);

    auto ioMessage1 = std::make_shared<IoMessage>(std::string("message"), 0, "g");
    port1->sendToExt(ioMessage1);

    auto poppedMessage = cellHub2->tryPop();
    ASSERT_THAT(poppedMessage->getCommand(), Eq("message"));
}

TEST(IoPortTest, IoPortExceptsOnMultipleConnects)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived1 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    io_ptr port1 = make_shared<IoPort>(cell_hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived2 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    io_ptr port2 = make_shared<IoPort>(cell_hub_ptr(cellHub2), msgsReceived2, "g");

    cell_hub_ptr cellHub3 = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived3 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();
    io_ptr port3 = make_shared<IoPort>(cell_hub_ptr(cellHub3), msgsReceived3, "g");

    port1->connect(port2);
    EXPECT_ANY_THROW(port1->connect(port3));
}

TEST(IoPortTest, IoPortAddsUuidToForwardedMessage)
{
    cell_hub_ptr cellHub = make_shared<CellHub>();
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsReceived1 = make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>();

    IoPort ioPort(cellHub, msgsReceived1, "g");

    auto ioMessage = std::make_shared<IoMessage>(std::string("message"), 0, std::string("g"));
    ioPort.fwdToQueue(ioMessage);

    auto poppedMessage = cellHub->tryPop();

    boost::uuids::uuid rcvPortUuid = poppedMessage->getForwardingPortUUID();
    boost::uuids::uuid ioPortUuid = ioPort.getUuid();

    ASSERT_EQ(rcvPortUuid, ioPortUuid);

}
