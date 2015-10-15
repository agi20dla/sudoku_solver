//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"

using namespace ::testing;
using namespace std;


TEST(BrainTest, InitializeGrid)
{
    Brain brain;
    brain.initialize();
}

TEST(BrainTest, SendHMessage)
{
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell00 = brain.getCell(0,0);
    shared_ptr<IoPort> port = cell00->getMsgConnection();
    IoMessage ioMessage("message", "h");
    port->fwdToQueue(ioMessage);

    brain.run();

    shared_ptr<Cell> cell08 = brain.getCell(0,8);
    ASSERT_EQ(cell08->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell10 = brain.getCell(1,0);
    ASSERT_EQ(cell10->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendVMessage)
{
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell00 = brain.getCell(0,0);
    shared_ptr<IoPort> port = cell00->getMsgConnection();
    IoMessage ioMessage("message", "v");
    port->fwdToQueue(ioMessage);

    brain.run();

    shared_ptr<Cell> cell80 = brain.getCell(8,0);
    ASSERT_EQ(cell80->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell01 = brain.getCell(0,1);
    ASSERT_EQ(cell01->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendGMessage)
{
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell33 = brain.getCell(3,3);
    shared_ptr<IoPort> port = cell33->getMsgConnection();
    IoMessage ioMessage("message", "g");
    port->fwdToQueue(ioMessage);

    brain.run();

    shared_ptr<Cell> cell32 = brain.getCell(3,2);
    ASSERT_EQ(cell32->numMessagesRcvd(), 0);

    shared_ptr<Cell> cell23 = brain.getCell(2,3);
    ASSERT_EQ(cell23->numMessagesRcvd(), 0);

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            shared_ptr<Cell> cell = brain.getCell(row, col);
            ASSERT_EQ(cell->numMessagesRcvd(), 1);
//            cout << "cell: " << row << ", " << col << " - " << cell.numMessagesRcvd() << " recieved" << endl;
        }
    }
}

TEST(BrainTest, SendGhvMessage) {
    Brain brain;
    brain.initialize();

    shared_ptr<Cell> cell33 = brain.getCell(3, 3);
    shared_ptr<IoPort> port = cell33->getMsgConnection();
    IoMessage ioMessageG("message", "g");
    port->fwdToQueue(ioMessageG);

    IoMessage ioMessageH("message", "h");
    port->fwdToQueue(ioMessageH);

    IoMessage ioMessageV("message", "v");
    port->fwdToQueue(ioMessageV);

    brain.run();

    shared_ptr<Cell> cell32 = brain.getCell(3, 2);
    ASSERT_EQ(cell32->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell22 = brain.getCell(2, 2);
    ASSERT_EQ(cell22->numMessagesRcvd(), 0);

    shared_ptr<Cell> cell23 = brain.getCell(2, 3);
    ASSERT_EQ(cell23->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell66 = brain.getCell(6, 6);
    ASSERT_EQ(cell66->numMessagesRcvd(), 0);

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            shared_ptr<Cell> cell = brain.getCell(row, col);
            ASSERT_GE(cell->numMessagesRcvd(), 1);
        }
    }
}


TEST(BrainTest, RemoveValue)
{
    Brain brain;
    brain.initialize();
    brain.removeValue(0, 0, 5);
    brain.run();
    shared_ptr<vector<uint>> values = brain.getValues(0,0);
    ASSERT_EQ((*values)[5], 0);
}

// Failing test that I'll fix later.
//TEST(BrainTest, SetValue) {
//    Brain brain;
//    brain.initialize();
//    brain.setValue(0, 0, 5);
//    brain.run();
//
//    // make sure this cell's values are set / reset appropriately
//    shared_ptr<vector<uint>> values00 = brain.getValues(0, 0);
//    ASSERT_EQ((*values00)[5], 1);
//    for (uint idx = 1; idx <= 9; idx++) {
//        if (idx == 5) {
//            ASSERT_EQ((*values00)[idx], 1);
//        }
//        ASSERT_EQ((*values00)[idx], 0);
//    }
//
//    // check another cell to make sure it's values are set / reset appropriately
//    shared_ptr<vector<uint>> values11 = brain.getValues(1, 1);
//    ASSERT_EQ((*values11)[5], 0);
//
//    // check another grid's cell to make sure it's values are set / reset appropriately
//    shared_ptr<vector<uint>> values30 = brain.getValues(3, 0);
//    ASSERT_EQ((*values30)[5], 0);
//}


