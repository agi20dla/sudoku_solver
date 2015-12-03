//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/PuzzleCell.h"

using namespace ::testing;
using namespace std;

TEST(CellTest, CellIsCreated)
{
    EXPECT_NO_THROW(PuzzleCell cell);
}

TEST(CellTest, GetNewConnectionToCell)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    auto port = cell->createPort("t");
    ASSERT_TRUE(port != nullptr);
}

TEST(CellTest, SendMessageThroughCell)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    auto port1 = cell->createPort("t");
    auto port2 = cell->createPort("t");

    IoMessage fwdMessage(std::string("message"), 0, "t");

    port1->fwdToQueue(fwdMessage);

    cell->run();

    ASSERT_THAT(port2->getNumMessagesRecieved(), Eq(1));
}


// p1 <-msg-> C1 <-msg-> p2 <-> p3 <-msg-> C2
TEST(CellTest, SendMessageThroughCellToAnotherCell)
{
    std::shared_ptr<PuzzleCell> cell1 = make_shared<PuzzleCell>();
    auto port1 = cell1->createPort("t");
    auto port2 = cell1->createPort("t");

    std::shared_ptr<PuzzleCell> cell2 = make_shared<PuzzleCell>();
    auto port3 = cell2->createPort("t");

    port2->connect(port3);
    port3->connect(port2);

    IoMessage message(std::string("message"), 0, "t");

    // simulate sending a message from another IoPort
    port1->fwdToQueue(message);

    cell1->run();
    cell2->run();

    ASSERT_EQ(1, port1->getNumMessagesRecieved());
    ASSERT_EQ(1, port1->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port1->getNumMessagesSent());

    ASSERT_EQ(1, port2->getNumMessagesRecieved());
    ASSERT_EQ(0, port2->getNumMsgsForwardedToHub());
    ASSERT_EQ(1, port2->getNumMessagesSent());

    ASSERT_EQ(1, port3->getNumMessagesRecieved());
    ASSERT_EQ(1, port3->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port3->getNumMessagesSent());
}


TEST(CellTest, CellsCanConnectToEachOther)
{
    std::shared_ptr<PuzzleCell> cell1 = make_shared<PuzzleCell>();
    std::shared_ptr<IoPort> port = cell1->createPort("t");
    std::shared_ptr<PuzzleCell> cell2 = make_shared<PuzzleCell>();

    cell1->connect(cell2, "t");

    IoMessage message(std::string("message"), 0, "t");
    port->fwdToQueue(message);
    cell1->run();

    ASSERT_EQ(cell2->numMessagesOnHub(), 1);
}


TEST(CellTest, GlobalCellDoesNotSendBackMessage)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    std::shared_ptr<IoPort> port = cell->createPort("g");

    IoMessage message(string("message"), 0, "g");

    port->fwdToQueue(message);
    ASSERT_EQ(1, port->getNumMessagesRecieved());
    ASSERT_EQ(1, port->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port->getNumMessagesSent());

    ASSERT_EQ(1, cell->numMessagesOnHub());

    cell->run();

    ASSERT_EQ(0, cell->numMessagesOnHub());

    ASSERT_EQ(1, port->getNumMessagesRecieved());
    ASSERT_EQ(1, port->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port->getNumMessagesSent());
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
    std::shared_ptr<PuzzleCell> tl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> global = make_shared<PuzzleCell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> tlMsgPort = tl->createPort("t");

    IoMessage message(string("message"), 0, "h");
    tlMsgPort->fwdToQueue(message);

    ASSERT_EQ(tl->numMessagesOnHub(), 1);

    tl->run();
    tr->run();
    bl->run();
    br->run();
    global->run();

    ASSERT_EQ(0, br->numMessagesSent());
    ASSERT_EQ(0, bl->numMessagesSent());
    ASSERT_EQ(0, tr->numMessagesSent());
    ASSERT_EQ(1, tl->numMessagesSent());
    ASSERT_EQ(0, global->numMessagesSent());

    ASSERT_EQ(1, tl->numMessagesRcvd()); // from the initial message forward
    ASSERT_EQ(0, br->numMessagesRcvd());
    ASSERT_EQ(0, bl->numMessagesRcvd());
    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesRcvd());

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
    std::shared_ptr<PuzzleCell> tl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> global = make_shared<PuzzleCell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> brMsgPort = br->createPort("t");

    IoMessage message(string("message"), 0, "v");
    brMsgPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessagesOnHub(), 1);

    br->run();
    tr->run();
    bl->run();
    tl->run();
    global->run();

    ASSERT_EQ(1, br->numMessagesSent());
    ASSERT_EQ(0, bl->numMessagesSent());
    ASSERT_EQ(0, tr->numMessagesSent());
    ASSERT_EQ(0, tl->numMessagesSent());
    ASSERT_EQ(0, global->numMessagesSent());

    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message forward
    ASSERT_EQ(0, tl->numMessagesRcvd());
    ASSERT_EQ(0, bl->numMessagesRcvd());
    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesRcvd());
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
    std::shared_ptr<PuzzleCell> tl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    std::shared_ptr<PuzzleCell> global = make_shared<PuzzleCell>();

    tl->connect(tr, "h");
    tl->connect(bl, "v");
    tl->connect(global, "g");

    tr->connect(br, "v");
    tr->connect(global, "g");

    br->connect(bl, "h");
    br->connect(global, "g");

    bl->connect(global, "g");

    // set up external connection
    std::shared_ptr<IoPort> extPort = br->createPort("t");

    IoMessage message(string("message"), 0, "g");
    extPort->fwdToQueue(message);

    ASSERT_EQ(br->numMessagesOnHub(), 1);

    br->run();
    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message insertion
    ASSERT_EQ(0, br->numMessagesOnHub());
    ASSERT_EQ(1, br->numMessagesSent());
    ASSERT_EQ(1, global->numMessagesOnHub());

    global->run();
    ASSERT_EQ(1, global->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesOnHub());

    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message insertion
    ASSERT_EQ(0, br->numMessagesOnHub());  // came from here, so don't send it back

    ASSERT_EQ(1, bl->numMessagesRcvd());
    ASSERT_EQ(1, bl->numMessagesOnHub());

    ASSERT_EQ(1, tl->numMessagesRcvd());
    ASSERT_EQ(1, tl->numMessagesOnHub());

    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(1, tr->numMessagesOnHub());

    tr->run();
    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(0, tr->numMessagesOnHub());

    ASSERT_EQ(1, global->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesOnHub());

    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message insertion
    ASSERT_EQ(0, br->numMessagesOnHub());

    ASSERT_EQ(1, bl->numMessagesRcvd());
    ASSERT_EQ(1, bl->numMessagesOnHub());

    ASSERT_EQ(1, tl->numMessagesRcvd());
    ASSERT_EQ(1, tl->numMessagesOnHub());

    bl->run();
    ASSERT_EQ(1, bl->numMessagesRcvd());
    ASSERT_EQ(0, bl->numMessagesOnHub());

    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(0, tr->numMessagesOnHub());

    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message insertion
    ASSERT_EQ(0, br->numMessagesOnHub());

    ASSERT_EQ(1, global->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesOnHub());

    ASSERT_EQ(1, tl->numMessagesRcvd());
    ASSERT_EQ(1, tl->numMessagesOnHub());

    tl->run();
    ASSERT_EQ(1, br->numMessagesRcvd()); // from the initial message insertion
    ASSERT_EQ(0, br->numMessagesOnHub());

    ASSERT_EQ(1, bl->numMessagesRcvd());
    ASSERT_EQ(0, bl->numMessagesOnHub());

    ASSERT_EQ(1, tl->numMessagesRcvd());
    ASSERT_EQ(0, tl->numMessagesOnHub());

    ASSERT_EQ(1, tr->numMessagesRcvd());
    ASSERT_EQ(0, tr->numMessagesOnHub());

    ASSERT_EQ(1, global->numMessagesRcvd());
    ASSERT_EQ(0, global->numMessagesOnHub());
}

TEST(CellTest, SetMessageIsProcessed)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    std::shared_ptr<IoPort> port = cell->createPort("m");

    IoMessage ioMessage(string("set"), 5, "g");

    port->fwdToQueue(ioMessage);

    cell->run();

    vector<uint> *values2 = cell->getPossibleValues();

    ASSERT_EQ(0, (*values2).at(4));
    ASSERT_EQ(1, (*values2).at(5));
}

TEST(CellTest, RmMessageIsProcessed)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    std::shared_ptr<IoPort> port = cell->createPort("m");

    IoMessage ioMessage(string("rm"), 5, "h");

    port->fwdToQueue(ioMessage);

    cell->run();

    vector<uint> *values = cell->getPossibleValues();
    uint value = (*values).at(5);

    ASSERT_EQ(0, value);
}
