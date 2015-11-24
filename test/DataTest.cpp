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
    ASSERT_EQ("53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79", data.getPuzzleStr());
}

TEST(DataTest, ReadsPuzzleFileIntoInt) {
    const string filename = "02_easy.pzl";

    Data data;
    data.init(filename);
    vector<uint> puzzle = {5, 3, 0, 0, 7, 0, 0, 0, 0, 6, 0, 0, 1, 9, 5, 0, 0, 0, 0, 9, 8, 0, 0, 0, 0, 6, 0, 8, 0, 0, 0,
                           6, 0, 0, 0, 3, 4, 0, 0, 8, 0, 3, 0, 0, 1, 7, 0, 0, 0, 2, 0, 0, 0, 6, 0, 6, 0, 0, 0, 0, 2, 8,
                           0, 0, 0, 0, 4, 1, 9, 0, 0, 5, 0, 0, 0, 0, 8, 0, 0, 7, 9};
    ASSERT_EQ(puzzle, data.getPuzzleVec());
}

TEST(DataTest, BadPuzzleFileThrowsException) {
    Data data;
    ASSERT_ANY_THROW(data.init("not_a_puzzle.pzl"));
}
