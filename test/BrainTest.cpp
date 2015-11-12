//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"

using namespace ::testing;
using namespace std;


TEST(BrainTest, InitializeGrid) {
    Brain brain;
    brain.initialize();
}

TEST(BrainTest, SendHMessage) {
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell00 = brain.getCell(0, 0);
    shared_ptr<IoPort> port = cell00->getMsgConnection();
    IoMessage ioMessage("message", "h");
    port->fwdToQueue(ioMessage);

    brain.run();

    shared_ptr<Cell> cell08 = brain.getCell(0, 8);
    ASSERT_EQ(cell08->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell10 = brain.getCell(1, 0);
    ASSERT_EQ(cell10->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendVMessage) {
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell00 = brain.getCell(0, 0);
    shared_ptr<IoPort> port = cell00->getMsgConnection();
    IoMessage ioMessage("message", "v");
    port->fwdToQueue(ioMessage);

    brain.run();

    shared_ptr<Cell> cell80 = brain.getCell(8, 0);
    ASSERT_EQ(cell80->numMessagesRcvd(), 1);

    shared_ptr<Cell> cell01 = brain.getCell(0, 1);
    ASSERT_EQ(cell01->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendGMessage) {
    Brain brain;
    brain.initialize();
    shared_ptr<Cell> cell33 = brain.getCell(3, 3);
    shared_ptr<IoPort> port = cell33->getMsgConnection();
    IoMessage ioMessage("message", "g");
    port->fwdToQueue(ioMessage);

    // Have to run the brain 3 times to ensure the messages get transferred where they need to go
    brain.run();
    brain.run();
    brain.run();

    shared_ptr<Cell> cell32 = brain.getCell(3, 2);
    ASSERT_EQ(cell32->numMessagesRcvd(), 0);

    shared_ptr<Cell> cell23 = brain.getCell(2, 3);
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

    // Have to run the brain 3 times to ensure the messages get transferred where they need to go
    brain.run();
    brain.run();
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


TEST(BrainTest, RemoveValue) {
    Brain brain;
    brain.initialize();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> values = brain.getValues(0, 0);
    ASSERT_EQ(values[5], 0);
}

TEST(BrainTest, RemoveSingleValueDoesntAffectOtherValues) {
    Brain brain;
    brain.initialize();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> values = brain.getValues(0, 0);
    ASSERT_EQ(values[4], 1);
    ASSERT_EQ(values[6], 1);
}

TEST(BrainTest, RemoveSingleValueDoesntAffectOtherCells) {
    Brain brain;
    brain.initialize();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> values10 = brain.getValues(1, 0);
    ASSERT_EQ(values10[5], 1);
    vector<uint> values01 = brain.getValues(0, 1);
    ASSERT_EQ(values01[5], 1);

}


TEST(BrainTest, SetValue) {
    Brain brain;
    brain.initialize();

    brain.setValue(0, 0, 5);
    brain.run();
    brain.printValues();
    for (int i = 0; i < 5; i++) {
        brain.run();
        brain.printValues();
    }
    brain.printMessagesRcvd();

    // check this block's cells to make sure it's values are set / reset appropriately
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // skip the cell we set, though
            if (row == 0 && col == 0) {
                vector<uint> values = brain.getValues(row, col);
                ASSERT_EQ(values[5], 1);
            } else {
                vector<uint> values = brain.getValues(row, col);
                ASSERT_EQ(values[5], 0);
            }
        }
    }

    // check the horizontal cells to make sure their values are set/reset appropriately
    for (int col = 1; col < 9; col++) {
        vector<uint> values = brain.getValues(0, col);
        ASSERT_EQ(values[5], 0);
    }

    // check the vertical cells to make sure their values are set/reset appropriately
    for (int row = 1; row < 9; row++) {
        vector<uint> values = brain.getValues(row, 0);
        ASSERT_EQ(values[5], 0);
    }
}


