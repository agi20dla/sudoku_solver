//
// Created by jason on 11/23/15.
//
#include <string>
#include "gmock/gmock.h"
#include "../src/Brain.h"
#include "../src/Data.h"

using namespace ::testing;
using namespace std;


TEST(SolutionTest, SolveEasyPuzzle02) {
    Data data;
    data.init("02_easy.pzl");
    Brain brain;
    brain.reset();
    brain.setValues(data.getDataAsVector());
    brain.solve(true);

    Data solution;
    solution.init("02_answer.pzl");

    Data puzzleSolution;
    puzzleSolution.init(brain.getSolution());

    ASSERT_TRUE(solution == puzzleSolution);
}
//
//
//TEST(SolutionTest, SolveEasyPuzzle30) {
//    Data data;
//    data.init("30_easy.pzl");
//    Brain brain;
//    brain.reset();
//    brain.setValues(data.getDataAsVector());
//    brain.solve(true);
//
//    Data solution;
//    solution.init("30_answer.pzl");
//
//    Data puzzleSolution;
//    puzzleSolution.init(brain.getSolution());
//
//    ASSERT_TRUE(solution == puzzleSolution);
//}
//
//
//TEST(SolutionTest, SolveMediumPuzzle32) {
//    Data data;
//    data.init("32_medium.pzl");
//    Brain brain;
//    brain.reset();
//    brain.setValues(data.getDataAsVector());
//    brain.solve(true);
//
//    Data solution;
//    solution.init("32_answer.pzl");
//
//    Data puzzleSolution;
//    puzzleSolution.init(brain.getSolution());
//
//    ASSERT_TRUE(solution == puzzleSolution);
//}


TEST(SolutionTest, SolveHardPuzzle67) {
    Data data;
    data.init("67_hard.pzl");
    Brain brain;
    brain.reset();
    brain.setValues(data.getDataAsVector());
    brain.solve(true);

    Data solution;
    solution.init("67_answer.pzl");

    Data puzzleSolution;
    puzzleSolution.init(brain.getSolution());

    ASSERT_TRUE(solution == puzzleSolution);
}

//
//TEST(SolutionTest, SolveDiabolicalPuzzle100) {
//    Data data;
//    data.init("100_diabolical.pzl");
//    Brain brain;
//    brain.reset();
//    brain.setValues(data.getDataAsVector());
//    brain.solve(true);
//
//    Data solution;
//    solution.init("100_answer.pzl");
//
//    Data puzzleSolution;
//    puzzleSolution.init(brain.getSolution());
//
//    ASSERT_TRUE(solution == puzzleSolution);
//}
