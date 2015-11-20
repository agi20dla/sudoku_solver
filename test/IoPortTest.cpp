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
//    std::shared_ptr<CellHub> hub = make_shared<CellHub>();
    auto hub = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();

    IoPort ioPort(hub, msgsReceived, "g");

    msg_ptr ioMessage = make_shared<IoMessage>("message");
    ioPort.fwdToQueue(ioMessage);

    msg_ptr poppedMessage = hub->tryPop();

    ASSERT_THAT(poppedMessage->getMessage(), Eq("message"));
}

TEST(IoPortTest, IoPortSendsAMessageToAnotherIoPort)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port1 = make_shared<IoPort>(hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived2 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port2 = make_shared<IoPort>(hub_ptr(cellHub2), msgsReceived2, "g");

    port1->connect(port2);
    port2->connect(port1);

    msg_ptr ioMessage1 = make_shared<IoMessage>("message");
    port1->sendToExt(ioMessage1);

    msg_ptr poppedMessage = cellHub2->tryPop();
    ASSERT_THAT(poppedMessage->getMessage(), Eq("message"));
}

TEST(IoPortTest, IoPortExceptsOnMultipleConnects)
{
    cell_hub_ptr cellHub1 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port1 = make_shared<IoPort>(hub_ptr(cellHub1), msgsReceived1, "g");

    cell_hub_ptr cellHub2 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived2 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port2 = make_shared<IoPort>(hub_ptr(cellHub2), msgsReceived2, "g");

    cell_hub_ptr cellHub3 = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived3 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();
    io_ptr port3 = make_shared<IoPort>(hub_ptr(cellHub3), msgsReceived3, "g");

    port1->connect(port2);
    EXPECT_ANY_THROW(port1->connect(port3));
}

TEST(IoPortTest, IoPortAddsUuidToForwardedMessage)
{
    cell_hub_ptr cellHub = make_shared<CellHub>();
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived1 = make_shared<boost::unordered_map<boost::uuids::uuid, uint>>();

    IoPort ioPort(cellHub, msgsReceived1, "g");

    msg_ptr ioMessage = make_shared<IoMessage>("message");
    ioPort.fwdToQueue(ioMessage);

    msg_ptr poppedMessage = cellHub->tryPop();

    boost::uuids::uuid rcvPortUuid = poppedMessage->getRcvPortUuid();
    boost::uuids::uuid ioPortUuid = ioPort.getUuid();

    ASSERT_EQ(rcvPortUuid, ioPortUuid);

}
