//
// Created by jason on 12/17/15.
//

#ifndef SUDOKU_SOLVER_SOLUTIONMATRIX_H
#define SUDOKU_SOLVER_SOLUTIONMATRIX_H

#include <vector>
#include "common.h"

/**
 * Stores the current best guess_ as to what a possible solution might be.  It adds positive (algorithm stalled)
 * and negative (algorithm encountered an error) sets of the grid.  The idea is that as the puzzle is processed,
 * it will generate positive and negative solutions.  The aggregate of those may help determine what the final
 * solution will be.
 */
class SolutionMatrix {
private:
    // 81 cell's worth of possible values
    // aggregates the sole value's in a positive or negative solution
    std::vector<std::vector<long>> guess_;

    uint getGuess(std::vector<long>);
public:
    SolutionMatrix();

    void addNegStates(vector<CellState> states );
    void addPosStates(vector<CellState> states );
    void printGuess();

};


#endif //SUDOKU_SOLVER_SOLUTIONMATRIX_H
