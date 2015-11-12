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
    shared_ptr<Cell> cell = make_shared<Cell>();
    auto port = cell->getMsgConnection();
    ASSERT_TRUE(port != nullptr);
}

TEST(CellTest, SendMessageThroughCell)
{
    shared_ptr<Cell> cell = make_shared<Cell>();
    auto port1 = cell->getMsgConnection();
    auto port2 = cell->getMsgConnection();

    IoMessage fwdMessage("message");

    port1->fwdToQueue(fwdMessage);

    cell->run();

    ASSERT_THAT(port2->getNumMessagesRecieved(), Eq(1));
}

TEST(CellTest, SendMessageThroughCellToAnotherCell)
{
    shared_ptr<Cell> cell1 = make_shared<Cell>();
    auto port1 = cell1->getMsgConnection();
    auto port2 = cell1->getMsgConnection();

    shared_ptr<Cell> cell2 = make_shared<Cell>();
    auto port3 = cell2->getMsgConnection();

    port2->connect(port3);
    port3->connect(port2);

    IoMessage message("message");

    // simulate sending a message from another IoPort
    port1->fwdToQueue(message);

    cell1->run();
    cell2->run();

    ASSERT_EQ(port1->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port1->getNumMessagesForwarded(), 1);
    ASSERT_EQ(port1->getNumMessagesSent(), 0);

    ASSERT_EQ(port2->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port2->getNumMessagesForwarded(), 0);
    ASSERT_EQ(port2->getNumMessagesSent(), 1);

    ASSERT_EQ(port3->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port3->getNumMessagesForwarded(), 1);
    ASSERT_EQ(port3->getNumMessagesSent(), 0);
}


TEST(CellTest, CellsCanConnectToEachOther)
{
    shared_ptr<Cell> cell1 = make_shared<Cell>();
    shared_ptr<IoPort> port = cell1->getMsgConnection();
    shared_ptr<Cell> cell2 = make_shared<Cell>();

    cell1->connect(cell2);

    IoMessage message("message");
    port->fwdToQueue(message);
    cell1->run();

    ASSERT_EQ(cell2->numMessages(), 1);
}


TEST(CellTest, GlobalCellDoesNotSendBackMessage)
{
    shared_ptr<Cell> cell = make_shared<Cell>();
    shared_ptr<IoPort> port = cell->getMsgConnection("g");

    IoMessage message("message", "g");

    port->fwdToQueue(message);
    ASSERT_EQ(port->getNumMessagesRecieved(), 1);
    ASSERT_EQ(port->getNumMessagesForwarded(), 1);
    ASSERT_EQ(port->getNumMessagesSent(), 0);

    ASSERT_EQ(cell->numMessages(), 1);

    cell->run();

    ASSERT_EQ(cell->numMessages(), 0);

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
    shared_ptr<Cell> tl = make_shared<Cell>();
    shared_ptr<Cell> tr = make_shared<Cell>();
    shared_ptr<Cell> bl = make_shared<Cell>();
    shared_ptr<Cell> br = make_shared<Cell>();
    shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    shared_ptr<IoPort> extPort = tl->getMsgConnection();

    IoMessage message("message", "h");
    extPort->fwdToQueue(message);

    ASSERT_EQ(tl->numMessages(), 1);

    tl->run();
    tr->run();
    bl->run();
    br->run();
    global->run();

    ASSERT_EQ(br->numMessagesSent(), 0);
    ASSERT_EQ(bl->numMessagesSent(), 0);
    ASSERT_EQ(tr->numMessagesSent(), 0);
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
    shared_ptr<Cell> tl = make_shared<Cell>();
    shared_ptr<Cell> tr = make_shared<Cell>();
    shared_ptr<Cell> bl = make_shared<Cell>();
    shared_ptr<Cell> br = make_shared<Cell>();
    shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    shared_ptr<IoPort> extPort = br->getMsgConnection();

    IoMessage message("message", "v");
    extPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessages(), 1);

    br->run();
    tr->run();
    bl->run();
    tl->run();
    global->run();

    ASSERT_EQ(br->numMessagesSent(), 1);
    ASSERT_EQ(bl->numMessagesSent(), 0);
    ASSERT_EQ(tr->numMessagesSent(), 0);
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
    shared_ptr<Cell> tl = make_shared<Cell>();
    shared_ptr<Cell> tr = make_shared<Cell>();
    shared_ptr<Cell> bl = make_shared<Cell>();
    shared_ptr<Cell> br = make_shared<Cell>();
    shared_ptr<Cell> global = make_shared<Cell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    shared_ptr<IoPort> extPort = br->getMsgConnection();

    IoMessage message("message", "g");
    extPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessages(), 1);

    br->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(br->numMessages(), 0);
    ASSERT_EQ(br->numMessagesSent(), 1);
    ASSERT_EQ(global->numMessages(), 1);

    global->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(br->numMessages(), 0);

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 1);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(tr->numMessages(), 1);

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(global->numMessages(), 0);

    tr->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(br->numMessages(), 0);

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 1);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(tr->numMessages(), 0);

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(global->numMessages(), 0);

    bl->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(br->numMessages(), 0);

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 0);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 1);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(tr->numMessages(), 0);

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(global->numMessages(), 0);

    tl->run();
    ASSERT_EQ(br->numMessagesRcvd(), 1); // from the initial message insertion
    ASSERT_EQ(br->numMessages(), 0);

    ASSERT_EQ(bl->numMessagesRcvd(), 1);
    ASSERT_EQ(bl->numMessages(), 0);

    ASSERT_EQ(tl->numMessagesRcvd(), 1);
    ASSERT_EQ(tl->numMessages(), 0);

    ASSERT_EQ(tr->numMessagesRcvd(), 1);
    ASSERT_EQ(tr->numMessages(), 0);

    ASSERT_EQ(global->numMessagesRcvd(), 1);
    ASSERT_EQ(global->numMessages(), 0);
}

TEST(CellTest, MgtRmMessageIsProcessed)
{
    shared_ptr<Cell> cell = make_shared<Cell>();
    shared_ptr<IoPort> mgtIo = cell->getMgtConnection("m");
    IoMessage ioMessage("rm:5", "h");
    mgtIo->fwdToQueue(ioMessage);
    cell->run();
    vector<uint> values = cell->getValues();
    uint value = values[5];

    ASSERT_EQ(value, 0);
}

TEST(CellTest, MgtSetMessageIsProcessed)
{
    shared_ptr<Cell> cell = make_shared<Cell>();
    shared_ptr<IoPort> msgIo = cell->getMgtConnection("m");
    IoMessage ioMessage("set:5", "g");
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
