//
// Created by jason on 4/17/18.
//

#ifndef SUDOKU_SOLVER_PUZZLE_H
#define SUDOKU_SOLVER_PUZZLE_H


#include <unordered_set>
#include <boost/uuid/uuid.hpp>
#include "common.h"
#include "ConcurrentQueue.h"

class Puzzle {

private:
    boost::uuids::uuid uuid_;

    std::vector<puzzle_cell_ptr> puzzleCells_;
    std::vector<global_cell_ptr> globalCells_;
    ConcurrentQueue<SolutionPath> solutionPaths_;
//    std::unordered_set<SolutionPath, StateHasher> solutionHashes_;
    std::vector<io_ptr> puzzlePorts_;
    bool firstRun_ = true;
    long numRuns_ = 0;
    long stalls_ = 0;
    long stalls_used_ = 0;
    long failures_ = 0;

    // Creates Global Cells and adds them to globalCells_
    void createGlobalCells();

    //Connects all the Global Cells to their intended Puzzle Cells
    void connectGlobals();

    // Initialize the Puzzle Cells to the given solution
    void setValues(vector<CellState> solution);

    void connectPuzzleToPuzzleCells();

    // Print the current calculated solution of the entire puzzle_ to cout
    // Will print values of 1-9, if that is the Puzzle Cell's sole value,
    // Otherwise, will print *
    void printSolution();

    std::vector<CellState> getSolutionStates();

    vector<CellValue> getPossibleSolutions();

public:

    Puzzle();

    Puzzle(ConcurrentQueue<SolutionPath> list);

    virtual ~Puzzle();

    int solve(bool useQueue = true);

    bool run();

    // Creates Puzzle Cells and adds them to puzzleCells_
    void createPuzzleCells();

    // Connects the Puzzle Cells in each row via an h direction port
    void connectPuzzleRows();

    // Connects the Puzzle Cells in each column via a v direction port
    void connectPuzzleCols();

    puzzle_cell_ptr getPuzzleCell(const uint row, const uint col);

    std::vector<puzzle_cell_ptr> getPuzzleCells();

    global_cell_ptr getGlobalCell(const uint row, const uint col);

    // set the value of a particular cell via a management message
    void setValue(const uint row, const uint col, const uint value);

    // Directly set the sole value and possible values, avoiding sending an "rm" message to all other cells
    void setValue(const uint row, const uint col, const uint value, const std::vector<uint> possibles);

    // Initialize the Puzzle Cells to the given values
    // Where a value is 1-9, and 0 represents a cell whose value hasn't been set
    void setValues(const vector<uint> vector);

    // Remove the given value from the Puzzle Cell at the given row and column
    void removeValue(const uint row, const uint col, const uint value);

    // Get the values from the Puzzle Cell at the given row and column
    vector<uint> *getValues(const uint row, const uint col);

    // Return true if all the Puzzle Cells contain a sole value
    bool isPuzzleSolved();

    std::vector<uint> getSolution();

    void reset();

    boost::uuids::uuid getUUID();

    io_ptr getPuzzlePort(const uint row, const uint col);
};


#endif //SUDOKU_SOLVER_PUZZLE_H
