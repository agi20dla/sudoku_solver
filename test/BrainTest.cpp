//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"

using namespace ::testing;

TEST(BrainTest, InitializeGrid) {
    Brain brain;
    brain.reset();
}

TEST(BrainTest, SendHMessage) {
    Brain brain;
    brain.reset();
    puzzle_cell_ptr cell00 = brain.getPuzzleCell(0, 0);
    io_ptr port = cell00->createPort("t");
    IoMessage ioMessage(string("message"), 0, "h");
    port->fwdToQueue(ioMessage);

    brain.run();

    puzzle_cell_ptr cell08 = brain.getPuzzleCell(0, 8);
    ASSERT_EQ(cell08->numMessagesRcvd(), 1);

    puzzle_cell_ptr cell10 = brain.getPuzzleCell(1, 0);
    ASSERT_EQ(cell10->numMessagesRcvd(), 0);
}

TEST(BrainTest, SendVMessage) {
    Brain brain;
    brain.reset();
    puzzle_cell_ptr cell00 = brain.getPuzzleCell(0, 0);
    io_ptr port = cell00->createPort("t");
    IoMessage ioMessage(string("message"), 0, "v");
    port->fwdToQueue(ioMessage);

    brain.run();

    puzzle_cell_ptr cell80 = brain.getPuzzleCell(8, 0);
    ASSERT_EQ(cell80->numMessagesRcvd(), 1);

    puzzle_cell_ptr cell01 = brain.getPuzzleCell(0, 1);
    ASSERT_EQ(cell01->numMessagesRcvd(), 0);
}

TEST(BrainTest, GlobalMessageStaysInCell) {
    Brain brain;
    brain.reset();
    puzzle_cell_ptr cell44 = brain.getPuzzleCell(4, 4);
    io_ptr port = cell44->createPort("t");
    IoMessage ioMessage(string("message"), 0, "g");
    port->fwdToQueue(ioMessage);

    brain.run(true);

    puzzle_cell_ptr cell22 = brain.getPuzzleCell(2, 2);
    ASSERT_EQ(0, cell22->numMessagesRcvd());

    puzzle_cell_ptr cell66 = brain.getPuzzleCell(6, 6);
    ASSERT_EQ(0, cell66->numMessagesRcvd());

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            puzzle_cell_ptr cell = brain.getPuzzleCell(row, col);
            ASSERT_EQ(1, cell->numMessagesRcvd()) << "row: " << to_string(row) << ", col: " << to_string(col);
        }
    }
}

TEST(BrainTest, SendGhvMessage) {
    Brain brain;
    brain.reset();

    puzzle_cell_ptr cell33 = brain.getPuzzleCell(3, 3);
    io_ptr port = cell33->createPort("t");
    IoMessage ioMessageG(string("message"), 0, "g");
    port->fwdToQueue(ioMessageG);

    IoMessage ioMessageH(string("message"), 0, "h");
    port->fwdToQueue(ioMessageH);

    IoMessage ioMessageV(string("message"), 0, "v");
    port->fwdToQueue(ioMessageV);

    brain.run(true);

    puzzle_cell_ptr cell32 = brain.getPuzzleCell(3, 2);
    ASSERT_EQ(cell32->numMessagesRcvd(), 1);

    puzzle_cell_ptr cell22 = brain.getPuzzleCell(2, 2);
    ASSERT_EQ(cell22->numMessagesRcvd(), 0);

    puzzle_cell_ptr cell23 = brain.getPuzzleCell(2, 3);
    ASSERT_EQ(cell23->numMessagesRcvd(), 1);

    puzzle_cell_ptr cell66 = brain.getPuzzleCell(6, 6);
    ASSERT_EQ(cell66->numMessagesRcvd(), 0);

    for (uint row = 3; row < 6; row++) {
        for (uint col = 3; col < 6; col++) {
            puzzle_cell_ptr cell = brain.getPuzzleCell(row, col);
            ASSERT_GE(cell->numMessagesRcvd(), 1) << "row: " << to_string(row) << ", col: " << to_string(col);
        }
    }
}


TEST(BrainTest, RemoveValue) {
    Brain brain;
    brain.reset();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> *values = brain.getValues(0, 0);
    uint value = (*values).at(5);
    ASSERT_EQ(0, value);
}

TEST(BrainTest, RemoveSingleValueDoesntAffectOtherValues) {
    Brain brain;
    brain.reset();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> *values = brain.getValues(0, 0);

    uint value4 = (*values)[4];
    ASSERT_EQ(1, value4);

    uint value6 = (*values)[6];
    ASSERT_EQ(1, value6);
}

TEST(BrainTest, RemoveSingleValueDoesntAffectOtherCells) {
    Brain brain;
    brain.reset();
    brain.removeValue(0, 0, 5);
    brain.run();
    vector<uint> *values10 = brain.getValues(1, 0);

    uint value105 = (*values10)[5];
    ASSERT_EQ(1, value105);

    vector<uint> *values01 = brain.getValues(0, 1);
    uint value015 = (*values01)[5];
    ASSERT_EQ(1, value015);

}


TEST(BrainTest, SetValue) {
    Brain brain;
    brain.reset();

    brain.setValue(0, 0, 5);
    brain.run(true);

    // check this block's cells to make sure it's values are set / reset appropriately
    for (uint row = 0; row < 3; row++) {
        for (uint col = 0; col < 3; col++) {
            // skip the cell we set, though
            if (row == 0 && col == 0) {
                vector<uint> *values = brain.getValues(row, col);
                uint value5 = (*values)[5];
                EXPECT_EQ(1, value5) << "row: " << row << ", col: " << col << endl;
            } else {
                vector<uint> *values = brain.getValues(row, col);
                uint value5 = (*values)[5];
                EXPECT_EQ(0, value5) << "row: " << row << ", col: " << col << endl;;
            }
        }
    }

    // check the horizontal cells to make sure their values are set/reset appropriately
    for (uint col = 1; col < 9; col++) {
        vector<uint> *values = brain.getValues(0, col);
        uint value5 = (*values)[5];
        ASSERT_EQ(0, value5);
    }

    // check the vertical cells to make sure their values are set/reset appropriately
    for (uint row = 1; row < 9; row++) {
        vector<uint> *values = brain.getValues(row, 0);
        uint value5 = (*values)[5];
        ASSERT_EQ(0, value5);
    }
}


TEST(BrainTest, SetTwoValuesInOneBlock) {
    Brain brain;
    brain.reset();

    brain.setValue(0, 0, 5);
    brain.setValue(2, 2, 9);
    brain.run(true);

    // check this block's cells to make sure it's values are set / reset appropriately
    for (uint row = 0; row < 3; row++) {
        for (uint col = 0; col < 3; col++) {
            // skip the cell we set, though
            vector<uint> *values = brain.getValues(row, col);
            if (row == 0 && col == 0) {
                uint value5 = (*values)[5];
                ASSERT_EQ(1, value5);
            } else if (row == 2 && col == 2) {
                uint value9 = (*values)[9];
                ASSERT_EQ(1, value9);
            } else {
                uint value5 = (*values)[5];
                ASSERT_EQ(0, value5);

                uint value9 = (*values)[9];
                ASSERT_EQ(0, value9);
            }
        }
    }

    // check the horizontal cells to make sure their values are set/reset appropriately
    for (uint col = 1; col < 9; col++) {
        vector<uint> *values = brain.getValues(0, col);
        uint value5 = (*values)[5];
        ASSERT_EQ(0, value5);
    }

    for (uint col = 0; col < 9; col++) {
        if (col == 2) {
            continue;
        }
        vector<uint> *values = brain.getValues(2, col);
        uint value9 = (*values)[9];
        ASSERT_EQ(0, value9) << "row: 2, col: " << to_string(col);
    }

    // check the vertical cells to make sure their values are set/reset appropriately
    for (uint row = 1; row < 9; row++) {
        vector<uint> *values = brain.getValues(row, 0);
        uint value5 = (*values)[5];
        ASSERT_EQ(0, value5);
    }
    for (uint row = 0; row < 9; row++) {
        if (row == 2) {
            continue;
        }
        vector<uint> *values = brain.getValues(row, 2);
        uint value9 = (*values)[9];
        ASSERT_EQ(0, value9);
    }
}


TEST(BrainTest, SetInitialValuesForPuzzle) {
    Brain brain;
    brain.reset();

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
}
