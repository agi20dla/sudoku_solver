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
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();

    IoPort ioPort(hub, msgsReceived, "g");

    IoMessage ioMessage(std::string("message"), 0, "t");
    ioPort.fwdToQueue(ioMessage);

    IoMessage poppedMessage;
    hub->tryPop(poppedMessage);

    ASSERT_THAT(poppedMessage.getMessage(), Eq("message"));
}

TEST(IoPortTest, IoPortSendsAMessageToAnotherIoPort)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port1 = make_shared<IoPort>(cell_hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived2 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port2 = make_shared<IoPort>(cell_hub_ptr(cellHub2), msgsReceived2, "g");

    port1->connect(port2);
    port2->connect(port1);

    IoMessage ioMessage1 = IoMessage(std::string("message"), 0, "g");
    port1->sendToExt(ioMessage1);

    IoMessage poppedMessage;
    cellHub2->tryPop(poppedMessage);
    ASSERT_THAT(poppedMessage.getMessage(), Eq("message"));
}

TEST(IoPortTest, IoPortExceptsOnMultipleConnects)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port1 = make_shared<IoPort>(cell_hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived2 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port2 = make_shared<IoPort>(cell_hub_ptr(cellHub2), msgsReceived2, "g");

    cell_hub_ptr cellHub3 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived3 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port3 = make_shared<IoPort>(cell_hub_ptr(cellHub3), msgsReceived3, "g");

    port1->connect(port2);
    EXPECT_ANY_THROW(port1->connect(port3));
}

TEST(IoPortTest, IoPortAddsUuidToForwardedMessage)
{
    cell_hub_ptr cellHub = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();

    IoPort ioPort(cellHub, msgsReceived1, "g");

    IoMessage ioMessage(std::string("message"), 0, "g");
    ioPort.fwdToQueue(ioMessage);

    IoMessage poppedMessage;
    cellHub->tryPop(poppedMessage);

    boost::uuids::uuid rcvPortUuid = poppedMessage.getRcvPortUuid();
    boost::uuids::uuid ioPortUuid = ioPort.getUuid();

    ASSERT_EQ(rcvPortUuid, ioPortUuid);

}
