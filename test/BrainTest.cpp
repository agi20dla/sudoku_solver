//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"

using namespace ::testing;

TEST(BrainTest, InitializeGrid) {
    Brain brain;
    brain.initialize();
}

TEST(BrainTest, SendHMessage) {
    Brain brain;
    brain.initialize();
    cell_ptr cell00 = brain.getCell(0, 0);
    io_ptr port = cell00->getMsgConnection();
    IoMessage ioMessage("message", 0, "h");
    port->fwdToQueue(ioMessage);

    brain.run();

    cell_ptr cell08 = brain.getCell(0, 8);
    ASSERT_EQ(cell08->numMessagesRcvd(), 1);

    cell_ptr cell10 = brain.getCell(1, 0);
    ASSERT_EQ(cell10->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendVMessage) {
    Brain brain;
    brain.initialize();
    cell_ptr cell00 = brain.getCell(0, 0);
    io_ptr port = cell00->getMsgConnection();
    IoMessage ioMessage("message", 0, "v");
    port->fwdToQueue(ioMessage);

    brain.run();

    cell_ptr cell80 = brain.getCell(8, 0);
    ASSERT_EQ(cell80->numMessagesRcvd(), 1);

    cell_ptr cell01 = brain.getCell(0, 1);
    ASSERT_EQ(cell01->numMessagesRcvd(), 0);
}

TEST(BrainTest, GlobalMessageStaysInCell) {
    Brain brain;
    brain.initialize();
    cell_ptr cell44 = brain.getCell(4, 4);
    io_ptr port = cell44->getMsgConnection();
    IoMessage ioMessage("message", 0, "g");
    port->fwdToQueue(ioMessage);

    brain.run(true);

    cell_ptr cell22 = brain.getCell(2, 2);
    ASSERT_EQ(0, cell22->numMessagesRcvd());

    cell_ptr cell66 = brain.getCell(6, 6);
    ASSERT_EQ(0, cell66->numMessagesRcvd());

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            cell_ptr cell = brain.getCell(row, col);
            ASSERT_EQ(1, cell->numMessagesRcvd()) << "row: " << to_string(row) << ", col: " << to_string(col);
        }
    }
}

TEST(BrainTest, SendGhvMessage) {
    Brain brain;
    brain.initialize();

    cell_ptr cell33 = brain.getCell(3, 3);
    io_ptr port = cell33->getMsgConnection();
    IoMessage ioMessageG("message", 0, "g");
    port->fwdToQueue(ioMessageG);

    IoMessage ioMessageH("message", 0, "h");
    port->fwdToQueue(ioMessageH);

    IoMessage ioMessageV("message", 0, "v");
    port->fwdToQueue(ioMessageV);

    brain.run(true);

    cell_ptr cell32 = brain.getCell(3, 2);
    ASSERT_EQ(cell32->numMessagesRcvd(), 1);

    cell_ptr cell22 = brain.getCell(2, 2);
    ASSERT_EQ(cell22->numMessagesRcvd(), 0);

    cell_ptr cell23 = brain.getCell(2, 3);
    ASSERT_EQ(cell23->numMessagesRcvd(), 1);

    cell_ptr cell66 = brain.getCell(6, 6);
    ASSERT_EQ(cell66->numMessagesRcvd(), 0);

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            cell_ptr cell = brain.getCell(row, col);
            ASSERT_GE(cell->numMessagesRcvd(), 1) << "row: " << to_string(row) << ", col: " << to_string(col);
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
    brain.run(true);

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


TEST(BrainTest, SetTwoValuesInOneBlock) {
    Brain brain;
    brain.initialize();

    brain.setValue(0, 0, 5);
    brain.setValue(2, 2, 9);
    brain.run(true);

    // check this block's cells to make sure it's values are set / reset appropriately
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // skip the cell we set, though
            vector<uint> values = brain.getValues(row, col);
            if (row == 0 && col == 0) {
                ASSERT_EQ(1, values[5]);
            } else if (row == 2 && col == 2) {
                ASSERT_EQ(1, values[9]);
            } else {
                ASSERT_EQ(0, values[5]);
                ASSERT_EQ(0, values[9]);
            }
        }
    }

    // check the horizontal cells to make sure their values are set/reset appropriately
    for (int col = 1; col < 9; col++) {
        vector<uint> valuesFive = brain.getValues(0, col);
        ASSERT_EQ(0, valuesFive[5]);
    }

    for (int col = 0; col < 9; col++) {
        if (col == 2) {
            continue;
        }
        vector<uint> valuesNine = brain.getValues(2, col);
        ASSERT_EQ(0, valuesNine[9]) << "row: 2, col: " << to_string(col);
    }

    // check the vertical cells to make sure their values are set/reset appropriately
    for (int row = 1; row < 9; row++) {
        vector<uint> valuesFive = brain.getValues(row, 0);
        ASSERT_EQ(0, valuesFive[5]);
    }
    for (int row = 0; row < 9; row++) {
        if (row == 2) {
            continue;
        }
        vector<uint> valuesNine = brain.getValues(row, 2);
        ASSERT_EQ(0, valuesNine[9]);
    }
}

TEST(BrainTest, SetInitialValuesForPuzzle) {
    Brain brain;
    brain.initialize();

    brain.setValue(0, 0, 5);
    brain.setValue(0, 1, 4);
    brain.setValue(0, 5, 9);
    brain.setValue(0, 8, 7);
    brain.setValue(1, 0, 2);
    brain.setValue(1, 2, 1);
    brain.setValue(1, 3, 3);
    brain.setValue(1, 5, 7);
    brain.setValue(1, 7, 8);
    brain.setValue(1, 8, 4);
    brain.setValue(2, 2, 6);
    brain.setValue(2, 3, 4);
    brain.setValue(2, 5, 5);
    brain.setValue(3, 2, 7);
    brain.setValue(4, 0, 4);
    brain.setValue(4, 1, 5);
    brain.setValue(4, 4, 9);
    brain.setValue(4, 7, 3);
    brain.setValue(4, 8, 1);
    brain.setValue(5, 6, 2);
    brain.setValue(6, 3, 9);
    brain.setValue(6, 5, 6);
    brain.setValue(6, 6, 4);
    brain.setValue(7, 0, 9);
    brain.setValue(7, 1, 1);
    brain.setValue(7, 3, 7);
    brain.setValue(7, 5, 4);
    brain.setValue(7, 6, 3);
    brain.setValue(7, 8, 8);
    brain.setValue(8, 0, 8);
    brain.setValue(8, 3, 2);
    brain.setValue(8, 7, 7);
    brain.setValue(8, 8, 5);

    brain.run(true);
//        cout << endl;
//        brain.printValues();
//        cout << endl;
//        brain.printSolution();
//        brain.printMessagesRcvd();
//    }
}


