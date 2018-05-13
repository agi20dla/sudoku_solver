//
// Created by jason on 5/23/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"

using namespace ::testing;

//TEST(BrainTest, InitializeGrid) {
//    Brain brain;
//    brain.reset();
//}
//
//TEST(BrainTest, SendHMessage) {
//    Puzzle brain;
//    brain.reset();
//    puzzle_cell_ptr cell00 = brain.getPuzzleCell(0, 0);
//    io_ptr port = cell00->createPort("t");
//    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("h"), brain.getUUID());
//    port->fwdToQueue(ioMessage);
//
//    brain.run();
//
//    puzzle_cell_ptr cell08 = brain.getPuzzleCell(0, 8);
//    ASSERT_EQ(cell08->numMessagesRcvd(), 1);
//
//    puzzle_cell_ptr cell10 = brain.getPuzzleCell(1, 0);
//    ASSERT_EQ(cell10->numMessagesRcvd(), 0);
//
//    port.reset();
//}
//
//TEST(BrainTest, SendVMessage) {
//    Puzzle brain;
//    brain.reset();
//    puzzle_cell_ptr cell00 = brain.getPuzzleCell(0, 0);
//    io_ptr port = cell00->createPort("t");
//    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("v"), brain.getUUID());
//    port->fwdToQueue(ioMessage);
//
//    brain.run();
//
//    puzzle_cell_ptr cell80 = brain.getPuzzleCell(8, 0);
//    ASSERT_EQ(cell80->numMessagesRcvd(), 1);
//
//    puzzle_cell_ptr cell01 = brain.getPuzzleCell(0, 1);
//    ASSERT_EQ(cell01->numMessagesRcvd(), 0);
//
//    port.reset();
//}
//
//TEST(BrainTest, GlobalMessageStaysInCell) {
//    Puzzle brain;
//    brain.reset();
//    puzzle_cell_ptr cell44 = brain.getPuzzleCell(4, 4);
//    io_ptr port = cell44->createPort("t");
//    auto ioMessage = std::make_shared<IoMessage>(string("message"), 0, std::string("g"), brain.getUUID());
//    port->fwdToQueue(ioMessage);
//
//    brain.run();
//
//    puzzle_cell_ptr cell22 = brain.getPuzzleCell(2, 2);
//    ASSERT_EQ(0, cell22->numMessagesRcvd());
//
//    puzzle_cell_ptr cell66 = brain.getPuzzleCell(6, 6);
//    ASSERT_EQ(0, cell66->numMessagesRcvd());
//
//    for (uint row = 3; row < 6; row++) {
//        for (uint col = 3; col < 6; col++) {
//            puzzle_cell_ptr cell = brain.getPuzzleCell(row, col);
//            ASSERT_EQ(1, cell->numMessagesRcvd());
//        }
//    }
//
//    port.reset();
//}
//
//TEST(BrainTest, SendGhvMessage) {
//    Puzzle brain;
//    brain.reset();
//
//    puzzle_cell_ptr cell33 = brain.getPuzzleCell(3, 3);
//    io_ptr port = cell33->createPort("t");
//    auto ioMessageG = std::make_shared<IoMessage>(string("message"), 0, std::string("g"), brain.getUUID());
//    port->fwdToQueue(ioMessageG);
//
//    auto ioMessageH = std::make_shared<IoMessage>(string("message"), 0, std::string("h"), brain.getUUID());
//    port->fwdToQueue(ioMessageH);
//
//    auto ioMessageV = std::make_shared<IoMessage>(string("message"), 0, std::string("v"), brain.getUUID());
//    port->fwdToQueue(ioMessageV);
//
//    brain.run();
//
//    puzzle_cell_ptr cell32 = brain.getPuzzleCell(3, 2);
//    ASSERT_EQ(cell32->numMessagesRcvd(), 1);
//
//    puzzle_cell_ptr cell22 = brain.getPuzzleCell(2, 2);
//    ASSERT_EQ(cell22->numMessagesRcvd(), 0);
//
//    puzzle_cell_ptr cell23 = brain.getPuzzleCell(2, 3);
//    ASSERT_EQ(cell23->numMessagesRcvd(), 1);
//
//    puzzle_cell_ptr cell66 = brain.getPuzzleCell(6, 6);
//    ASSERT_EQ(cell66->numMessagesRcvd(), 0);
//
//    for (uint row = 3; row < 6; row++) {
//        for (uint col = 3; col < 6; col++) {
//            puzzle_cell_ptr cell = brain.getPuzzleCell(row, col);
//            ASSERT_GE(cell->numMessagesRcvd(), 1);
//        }
//    }
//
//    port.reset();
//}
//
//
//TEST(BrainTest, RemoveValue) {
//    Puzzle brain;
//    brain.reset();
//    brain.removeValue(0, 0, 5);
//    brain.run();
//    vector<uint> *values = brain.getValues(0, 0);
//    uint value = (*values).at(5);
//    ASSERT_EQ(0, value);
//}
//
//TEST(BrainTest, RemoveSingleValueDoesntAffectOtherValues) {
//    Puzzle brain;
//    brain.reset();
//    brain.removeValue(0, 0, 5);
//    brain.run();
//    vector<uint> *values = brain.getValues(0, 0);
//
//    uint value4 = (*values)[4];
//    ASSERT_EQ(1, value4);
//
//    uint value6 = (*values)[6];
//    ASSERT_EQ(1, value6);
//}
//
//TEST(BrainTest, RemoveSingleValueDoesntAffectOtherCells) {
//    Puzzle brain;
//    brain.reset();
//    brain.removeValue(0, 0, 5);
//    brain.run();
//    vector<uint> *values10 = brain.getValues(1, 0);
//
//    uint value105 = (*values10)[5];
//    ASSERT_EQ(1, value105);
//
//    vector<uint> *values01 = brain.getValues(0, 1);
//    uint value015 = (*values01)[5];
//    ASSERT_EQ(1, value015);
//
//}
//
//
//TEST(BrainTest, SetValue) {
//    Puzzle brain;
//    brain.reset();
//
//    brain.setValue(0, 0, 5);
//    brain.run();
//
//    // check this block's cells to make sure it's values are set / reset appropriately
//    for (uint row = 0; row < 3; row++) {
//        for (uint col = 0; col < 3; col++) {
//            // skip the cell we set, though
//            if (row == 0 && col == 0) {
//                vector<uint> *values = brain.getValues(row, col);
//                uint value5 = (*values)[5];
//                EXPECT_EQ(1, value5);
//            } else {
//                vector<uint> *values = brain.getValues(row, col);
//                uint value5 = (*values)[5];
//                EXPECT_EQ(0, value5);
//            }
//        }
//    }
//
//    // check the horizontal cells to make sure their values are set/reset appropriately
//    for (uint col = 1; col < 9; col++) {
//        vector<uint> *values = brain.getValues(0, col);
//        uint value5 = (*values)[5];
//        ASSERT_EQ(0, value5);
//    }
//
//    // check the vertical cells to make sure their values are set/reset appropriately
//    for (uint row = 1; row < 9; row++) {
//        vector<uint> *values = brain.getValues(row, 0);
//        uint value5 = (*values)[5];
//        ASSERT_EQ(0, value5);
//    }
//}
//
//
//TEST(BrainTest, SetTwoValuesInOneBlock) {
//    Puzzle brain;
//    brain.reset();
//
//    brain.setValue(0, 0, 5);
//    brain.setValue(2, 2, 9);
//    brain.run();
//
//    // check this block's cells to make sure it's values are set / reset appropriately
//    for (uint row = 0; row < 3; row++) {
//        for (uint col = 0; col < 3; col++) {
//            // skip the cell we set, though
//            vector<uint> *values = brain.getValues(row, col);
//            if (row == 0 && col == 0) {
//                uint value5 = (*values)[5];
//                ASSERT_EQ(1, value5);
//            } else if (row == 2 && col == 2) {
//                uint value9 = (*values)[9];
//                ASSERT_EQ(1, value9);
//            } else {
//                uint value5 = (*values)[5];
//                ASSERT_EQ(0, value5);
//
//                uint value9 = (*values)[9];
//                ASSERT_EQ(0, value9);
//            }
//        }
//    }
//
//    // check the horizontal cells to make sure their values are set/reset appropriately
//    for (uint col = 1; col < 9; col++) {
//        vector<uint> *values = brain.getValues(0, col);
//        uint value5 = (*values)[5];
//        ASSERT_EQ(0, value5);
//    }
//
//    for (uint col = 0; col < 9; col++) {
//        if (col == 2) {
//            continue;
//        }
//        vector<uint> *values = brain.getValues(2, col);
//        uint value9 = (*values)[9];
//        ASSERT_EQ(0, value9);
//    }
//
//    // check the vertical cells to make sure their values are set/reset appropriately
//    for (uint row = 1; row < 9; row++) {
//        vector<uint> *values = brain.getValues(row, 0);
//        uint value5 = (*values)[5];
//        ASSERT_EQ(0, value5);
//    }
//    for (uint row = 0; row < 9; row++) {
//        if (row == 2) {
//            continue;
//        }
//        vector<uint> *values = brain.getValues(row, 2);
//        uint value9 = (*values)[9];
//        ASSERT_EQ(0, value9);
//    }
//}
