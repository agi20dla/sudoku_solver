//
// Created by jason on 11/23/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"
#include "../src/Data.h"

using namespace ::testing;
using namespace std;


TEST(BrainTest, SolveEasyPuzzle1) {
    Data data;
    data.init("02_easy.pzl");
    Brain brain;
    brain.reset();
    brain.initialize(data.getPuzzleVec());
    brain.run(true);

    Data solution;
    solution.init("02_answer.pzl");

    Data puzzleSolution;
    puzzleSolution.init(brain.getSolution());

    ASSERT_TRUE(solution == puzzleSolution);
}


TEST(BrainTest, SolveEasyPuzzle2) {
    Data data;
    data.init("30_easy.pzl");
    Brain brain;
    brain.reset();
    brain.initialize(data.getPuzzleVec());
    brain.run(true);

    Data solution;
    solution.init("03_answer.pzl");

    Data puzzleSolution;
    puzzleSolution.init(brain.getSolution());

    ASSERT_TRUE(solution == puzzleSolution);
}


//TEST(BrainTest, SolveMediumPuzzle) {
//    Data data;
//    data.init("32_medium.pzl");
//    Brain brain;
//    brain.reset();
//    brain.initialize(data.getPuzzleVec());
//    brain.run(true);
//}
