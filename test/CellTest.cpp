//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Cell.h"

using namespace ::testing;
using namespace std;

TEST(CellTest, CellIsCreated)
{
    EXPECT_NO_THROW(Cell cell);
}

TEST(CellTest, GetNewConnectionToCell)
{
    std::shared_ptr<Cell> cell = make_shared<Cell>();
    auto port = cell->getMsgConnection();
    ASSERT_TRUE(port != nullptr);
}

TEST(CellTest, SendMessageThroughCell)
{
    std::shared_ptr<Cell> cell = make_shared<Cell>();
    auto port1 = cell->getMsgConnection();
    auto port2 = cell->getMsgConnection();

    msg_ptr fwdMessage = make_shared<IoMessage>("message");

    port1->fwdToQueue(fwdMessage);

    cell->run();

    ASSERT_THAT(port2->getNumMessagesRecieved(), Eq(1));
}


// p1 <-msg-> C1 <-msg-> p2 <-> p3 <-msg-> C2
TEST(CellTest, SendMessageThroughCellToAnotherCell)
{
    std::shared_ptr<Cell> cell1 = make_shared<Cell>();
    auto port1 = cell1->getMsgConnection();
    auto port2 = cell1->getMsgConnection();

    std::shared_ptr<Cell> cell2 = make_shared<Cell>();
    auto port3 = cell2->getMsgConnection();

    port2->connect(port3);
    port3->connect(port2);

    msg_ptr message = make_shared<IoMessage>("message");

    // simulate sending a message from another IoPort
    port1->fwdToQueue(message);

    cell1->run();
    cell2->run();

    ASSERT_EQ(1, port1->getNumMessagesRecieved());
    ASSERT_EQ(1, port1->getNumMessagesForwarded());
    ASSERT_EQ(0, port1->getNumMessagesSent());

    ASSERT_EQ(1, port2->getNumMessagesRecieved());
    ASSERT_EQ(0, port2->getNumMessagesForwarded());
    ASSERT_EQ(1, port2->getNumMessagesSent());

    ASSERT_EQ(2, port3->getNumMessagesRecieved());
    ASSERT_EQ(1, port3->getNumMessagesForwarded());
    ASSERT_EQ(1, port3->getNumMessagesSent());
}


TEST(CellTest, CellsCanConnectToEachOther)
{
    std::shared_ptr<Cell> cell1 = make_shared<Cell>();
    std::shared_ptr<IoPort> port = cell1->getMsgConnection();
    std::shared_ptr<Cell> cell2 = make_shared<Cell>();

    cell1->connect(cell2);

    msg_ptr message = make_shared<IoMessage>("message");
    port->fwdToQueue(message);
    cell1->run();

    ASSERT_EQ(cell2->numMessages(), 1);
}


TEST(CellTest, GlobalCellDoesNotSendBackMessage)
{
    std::shared_ptr<Cell> cell = make_shared<Cell>();
    std::shared_ptr<IoPort> port = cell->getMsgConnection("g");

    msg_ptr message = make_shared<IoMessage>("message", 0, "g");

    port->fwdToQueue(message);
    ASSERT_EQ(port->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port->getNumMessagesForwarded(), 1);
    ASSERT_EQ(port->getNumMessagesSent(), 0);

    ASSERT_EQ(cell->numMessages(), 1);

    cell->run();

    ASSERT_EQ(1, cell->numMessages());

    ASSERT_EQ(port->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port->getNumMessagesForwarded(), 1);
    ASSERT_EQ(port->getNumMessagesSent(), 0);
}

/**
 * With this grid setup:
 *
 * C h C
 * v G v
 * C h C
 *
 * Where "C" cells are cells that maintain state of the puzzle
 * and "G" cells are special global cells that allow "C" cells
 * to connect to each other without redundant connections
 */
TEST(CellTest, MessageOnlyGoesHorizontally)
{
    // set up grid
    std::shared_ptr<Cell> tl = make_shared<Cell>();
    std::shared_ptr<Cell> tr = make_shared<Cell>();
    std::shared_ptr<Cell> bl = make_shared<Cell>();
    std::shared_ptr<Cell> br = make_shared<Cell>();
    std::shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> extPort = tl->getMsgConnection();

    msg_ptr message = make_shared<IoMessage>("message", 0, "h");
    extPort->fwdToQueue(message);

    ASSERT_EQ(tl->numMessages(), 1);

    tl->run();
    tr->run();
    bl->run();
    br->run();
    global->run();

    ASSERT_EQ(br->numMessagesSent(), 0);
    ASSERT_EQ(bl->numMessagesSent(), 0);
    ASSERT_EQ(1, tr->numMessagesSent());
    ASSERT_EQ(tl->numMessagesSent(), 1);

    ASSERT_EQ(tl->numMessagesRcvd(), 1); // from the initial message forward
    ASSERT_EQ(br->numMessagesRcvd(), 0);
    ASSERT_EQ(bl->numMessagesRcvd(), 0);
    ASSERT_EQ(tr->numMessagesRcvd(), 1);

}


/**
 * With this grid setup:
 *
 * C h C
 * v G v
 * C h C
 *
 * Where "C" cells are cells that maintain state of the puzzle
 * and "G" cells are special global cells that allow "C" cells
 * to connect to each other without redundant connections
 */
TEST(CellTest, MessageOnlyGoesVertically)
{
    // set up grid
    std::shared_ptr<Cell> tl = make_shared<Cell>();
    std::shared_ptr<Cell> tr = make_shared<Cell>();
    std::shared_ptr<Cell> bl = make_shared<Cell>();
    std::shared_ptr<Cell> br = make_shared<Cell>();
    std::shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> extPort = br->getMsgConnection();

    msg_ptr message = make_shared<IoMessage>("message", 0, "v");
    extPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessages(), 1);

    br->run();
    tr->run();
    bl->run();
    tl->run();
    global->run();

    ASSERT_EQ(br->numMessagesSent(), 1);
    ASSERT_EQ(bl->numMessagesSent(), 0);
    ASSERT_EQ(1, tr->numMessagesSent());
    ASSERT_EQ(tl->numMessagesSent(), 0);

    ASSERT_EQ(tl->numMessagesRcvd(), 0);
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message forward
    ASSERT_EQ(bl->numMessagesRcvd(), 0);
    ASSERT_EQ(tr->numMessagesRcvd(), 1);

}


/**
 * With this grid setup:
 *
 * C h C
 * v G v
 * C h C
 *
 * Where "C" cells are cells that maintain state of the puzzle
 * and "G" cells are special global cells that allow "C" cells
 * to connect to each other without redundant connections
 */
TEST(CellTest, MessageGoesGlobal)
{
    // set up grid
    std::shared_ptr<Cell> tl = make_shared<Cell>();
    std::shared_ptr<Cell> tr = make_shared<Cell>();
    std::shared_ptr<Cell> bl = make_shared<Cell>();
    std::shared_ptr<Cell> br = make_shared<Cell>();
    std::shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> extPort = br->getMsgConnection();

    msg_ptr message = make_shared<IoMessage>("message", 0, "g");
    extPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessages(), 1);

    br->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(1, br->numMessages());
    ASSERT_EQ(br->numMessagesSent(), 1);
    ASSERT_EQ(global->numMessages(), 1);

    global->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(1, br->numMessages());

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 1);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(tr->numMessages(), 1);

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(1, global->numMessages());

    tr->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(1, br->numMessages());

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 1);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(1, tr->numMessages());

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(1, global->numMessages());

    bl->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(1, br->numMessages());

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(1, bl->numMessages());

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(1, tr->numMessages());

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(1, global->numMessages());

    tl->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(1, br->numMessages());

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(1, bl->numMessages());

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(1, tl->numMessages());

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(1, tr->numMessages());

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(1, global->numMessages());
}

TEST(CellTest, MgtRmMessageIsProcessed)
{
    std::shared_ptr<Cell> cell = make_shared<Cell>();
    std::shared_ptr<IoPort> mgtIo = cell->getMgtConnection("m");
    msg_ptr ioMessage = make_shared<IoMessage>("rm", 5, "h");
    mgtIo->fwdToQueue(ioMessage);
    cell->run();
    vector<uint> values = cell->getValues();
    uint value = values[5];

    ASSERT_EQ(value, 0);
}

TEST(CellTest, MgtSetMessageIsProcessed)
{
    std::shared_ptr<Cell> cell = make_shared<Cell>();
    std::shared_ptr<IoPort> msgIo = cell->getMgtConnection("m");
    msg_ptr ioMessage = make_shared<IoMessage>("set", 5, "g");
    msgIo->fwdToQueue(ioMessage);
    cell->run();
    vector<uint> values1 = cell->getValues();

    // necessary because msg is run before mgt and
    // mgt needs to be run after msg is run
    cell->run();
    vector<uint> values2 = cell->getValues();

    ASSERT_EQ(values2[4], 0);
    ASSERT_EQ(values2[5], 1);
}
