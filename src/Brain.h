//
// Created by jason on 6/5/15.
//

#ifndef SUDOKU_SOLVER_BRAIN_H
#define SUDOKU_SOLVER_BRAIN_H

#include <vector>
#include <queue>
#include <list>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include "IoPort.h"
#include "PuzzleCell.h"
#include "common.h"
#include "Puzzle.h"


class Brain {
private:
    boost::uuids::uuid uuid_;
    std::vector<io_ptr> brainPorts_;

    ConcurrentQueue<SolutionPath> solutionPaths_;
//    std::unordered_set<SolutionPath, StateHasher> solutionHashes_;
//    ConcurrentQueue<Puzzle> puzzleQueue_;
public:
    Brain();
    virtual ~Brain();

    /**
    * Reset a sudoku brain to the below map.
    * C = Puzzle Cell
    * G = Global Cell (Not shown.  Each block contains one Global cell connecting all it's Puzzle Cells)
    * h = Horizontal port connection
    * v = Vertical port connection
    * g = Not shown. Connection between Global Cells and a block's Puzzle Cells
    *
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C 
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C 
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C
    *    v   v   v     v   v   v     v   v   v
    *    C h C h C  h  C h C h C  h  C h C h C 
    */
    void reset();

    io_ptr getBrainPort(const uint row, const uint col);

    // Run until there's nothing else to do
    // Set debug to true to output state information as it runs
    int solve(const vector<uint> vector);

    // Print the current calculated solution of the entire puzzle_ to cout
    // Will print values of 1-9, if that is the Puzzle Cell's sole value,
    // Otherwise, will print *
    void printSolution();

    boost::uuids::uuid getUUID();
};


#endif //SUDOKU_SOLVER_BRAIN_H
