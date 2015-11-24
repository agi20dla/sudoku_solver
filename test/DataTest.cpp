//
// Created by jason on 11/22/15.
//


#include "gmock/gmock.h"
#include "../src/Data.h"

using namespace std;

TEST(DataTest, ReadsPuzzleFileIntoStr) {
    const string filename = "02_easy.pzl";

    Data data;
    data.init(filename);

    ASSERT_EQ("54...9..72.13.7.84..64.5.....7......45..9..31......2.....9.64..91.7.43.88..2...75", data.getPuzzleStr());
}

TEST(DataTest, ReadsPuzzleFileIntoInt) {
    const string filename = "02_easy.pzl";

    Data data;
    data.init(filename);

    vector<uint> puzzle = {5, 4, 0, 0, 0, 9, 0, 0, 7, 2, 0, 1, 3, 0, 7, 0, 8, 4, 0, 0, 6, 4, 0, 5, 0, 0, 0, 0, 0, 7, 0,
                           0, 0, 0, 0, 0, 4, 5, 0, 0, 9, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 9, 0, 6, 4, 0,
                           0, 9, 1, 0, 7, 0, 4, 3, 0, 8, 8, 0, 0, 2, 0, 0, 0, 7, 5};
    ASSERT_EQ(puzzle, data.getPuzzleVec());
}

TEST(DataTest, BadPuzzleFileThrowsException) {
    Data data;
    ASSERT_ANY_THROW(data.init("not_a_puzzle.pzl"));
}
