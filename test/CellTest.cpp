//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Cell.h"
#include "../src/GlobalCell.h"
#include "../src/PuzzleCell.h"
#include "../src/Random.h"

using namespace ::testing;
using namespace std;

TEST(CellTest, CellIsCreated)
{
    EXPECT_NO_THROW(PuzzleCell cell);
}

TEST(CellTest, GetNewConnectionToCell)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    cell->init();
    auto port = cell->createPort(string("t"));
    ASSERT_TRUE(port != nullptr);
    port.reset();
}

TEST(CellTest, SendMessageThroughCell)
{
    std::shared_ptr<PuzzleCell> cell1 = make_shared<PuzzleCell>();
    cell1->init();
    auto port1 = cell1->createPort("t");

    std::shared_ptr<PuzzleCell> cell2 = make_shared<PuzzleCell>();
    cell2->init();
    auto port2 = cell2->createPort("t");

    port1->connect(port2);
    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("t"), uuid);

    port1->fwdToQueue(ioMessage);

    cell1->run();

    ASSERT_EQ(1, port2->getNumMessagesRecieved());

    port1.reset();
    port2.reset();
}


// p1 <-msg-> C1 <-msg-> p2 <-> p3 <-msg-> C2
TEST(CellTest, SendMessageThroughCellToAnotherCell)
{
    std::shared_ptr<PuzzleCell> cell1 = make_shared<PuzzleCell>();
    cell1->init();
    auto port1 = cell1->createPort(string("t"));
    auto port2 = cell1->createPort(string("t"));

    std::shared_ptr<PuzzleCell> cell2 = make_shared<PuzzleCell>();
    cell2->init();
    auto port3 = cell2->createPort(string("t"));

    port2->connect(port3);
    port3->connect(port2);

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("t"), uuid);

    // simulate sending a message from another IoPort
    port1->fwdToQueue(ioMessage);

    cell1->run();
    cell2->run();

    ASSERT_EQ(1, port1->getNumMessagesRecieved());
    ASSERT_EQ(1, port1->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port1->getNumMessagesSent());

    ASSERT_EQ(0, port2->getNumMessagesRecieved());
    ASSERT_EQ(0, port2->getNumMsgsForwardedToHub());
    ASSERT_EQ(1, port2->getNumMessagesSent());

    ASSERT_EQ(1, port3->getNumMessagesRecieved());
    ASSERT_EQ(1, port3->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port3->getNumMessagesSent());

    port1.reset();
    port2.reset();
    port3.reset();
}


TEST(CellTest, CellsCanConnectToEachOther)
{
    std::shared_ptr<PuzzleCell> cell1 = make_shared<PuzzleCell>();
    cell1->init();
    std::shared_ptr<IoPort> port = cell1->createPort("t");
    std::shared_ptr<PuzzleCell> cell2 = make_shared<PuzzleCell>();
    cell2->init();

    cell1->connect(cell2, string("t"));

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("t"), uuid);
    port->fwdToQueue(ioMessage);
    cell1->run();

    ASSERT_EQ(cell2->numMessagesOnHub(), 1);

    port.reset();
}


TEST(CellTest, GlobalCellDoesNotSendBackMessage)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    cell->init();
    std::shared_ptr<IoPort> port = cell->createPort(string("g"));

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("g"), uuid);

    port->fwdToQueue(ioMessage);
    ASSERT_EQ(1, port->getNumMessagesRecieved());
    ASSERT_EQ(1, port->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port->getNumMessagesSent());

    ASSERT_EQ(1, cell->numMessagesOnHub());

    cell->run();

    ASSERT_EQ(0, cell->numMessagesOnHub());

    ASSERT_EQ(1, port->getNumMessagesRecieved());
    ASSERT_EQ(1, port->getNumMsgsForwardedToHub());
    ASSERT_EQ(0, port->getNumMessagesSent());

    port.reset();
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
    tl->init();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    tr->init();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    bl->init();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    br->init();
    std::shared_ptr<GlobalCell> global = make_shared<GlobalCell>();

    tl->connect(tr, string("h"));
    tl->connect(bl, string("v"));
    tl->connect(static_pointer_cast<Cell>(global), string("g"));

    tr->connect(br, string("v"));
    tr->connect(static_pointer_cast<Cell>(global), string("g"));

    br->connect(bl, string("h"));
    br->connect(static_pointer_cast<Cell>(global), string("g"));

    bl->connect(static_pointer_cast<Cell>(global), string("g"));


    // set up external connection
    std::shared_ptr<IoPort> tlMsgPort = tl->createPort("t");

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("h"), uuid);
    tlMsgPort->fwdToQueue(ioMessage);

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

    tlMsgPort.reset();
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
    tl->init();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    tr->init();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    bl->init();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    br->init();
    std::shared_ptr<GlobalCell> global = make_shared<GlobalCell>();

    tl->connect(tr, string("h"));
    tl->connect(bl, string("v"));
    tl->connect(static_pointer_cast<Cell>(global), string("g"));

    tr->connect(br, string("v"));
    tr->connect(static_pointer_cast<Cell>(global), string("g"));

    br->connect(bl, string("h"));
    br->connect(static_pointer_cast<Cell>(global), string("g"));

    bl->connect(static_pointer_cast<Cell>(global), string("g"));

    // set up external connection
    std::shared_ptr<IoPort> brMsgPort = br->createPort("t");

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("v"), uuid);
    brMsgPort->fwdToQueue(ioMessage);

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

    brMsgPort.reset();
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
    tl->init();
    std::shared_ptr<PuzzleCell> tr = make_shared<PuzzleCell>();
    tr->init();
    std::shared_ptr<PuzzleCell> bl = make_shared<PuzzleCell>();
    bl->init();
    std::shared_ptr<PuzzleCell> br = make_shared<PuzzleCell>();
    br->init();
    std::shared_ptr<GlobalCell> global = make_shared<GlobalCell>();


    tl->connect(tr, string("h"));
    tl->connect(bl, string("v"));
    tl->connect(global, string("g"));

    tr->connect(br, string("v"));
    tr->connect(global, string("g"));

    br->connect(bl, string("h"));
    br->connect(global, string("g"));

    bl->connect(global, string("g"));

    // set up external connection
    std::shared_ptr<IoPort> extPort = br->createPort(string("t"));

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("g"), uuid);
    extPort->fwdToQueue(ioMessage);

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

    br.reset();
}

TEST(CellTest, SetMessageIsProcessed)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    cell->init();
    std::shared_ptr<IoPort> port = cell->createPort(string("m"));

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("set"), 5, std::string("g"), uuid);

    port->fwdToQueue(ioMessage);

    cell->run();

    vector<uint> *values2 = cell->getPossibleValues();

    ASSERT_EQ(0, (*values2).at(4));
    ASSERT_EQ(1, (*values2).at(5));

    port.reset();
}

TEST(CellTest, RmMessageIsProcessed)
{
    std::shared_ptr<PuzzleCell> cell = make_shared<PuzzleCell>();
    cell->init();
    std::shared_ptr<IoPort> port = cell->createPort(string("m"));

    auto uuid = Random::getInstance().getNewUUID();
    auto ioMessage = std::make_shared<IoMessage>(string("rm"), 5, std::string("h"), uuid);

    port->fwdToQueue(ioMessage);

    cell->run();

    vector<uint> *values = cell->getPossibleValues();
    uint value = (*values).at(5);

    ASSERT_EQ(0, value);

    port.reset();
}
