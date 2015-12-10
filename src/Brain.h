//
// Created by jason on 6/5/15.
//

#ifndef SUDOKU_SOLVER_BRAIN_H
#define SUDOKU_SOLVER_BRAIN_H

#include <vector>
#include "IoPort.h"
#include "PuzzleCell.h"
#include "common.h"

struct valStruct {
    uint row;
    uint col;
    uint val;
};

struct puzzleState {
    uint row;
    uint col;
    uint soleValue;
    vector<uint> possibles;
};

struct solutionPath {
    vector<puzzleState> states;
    vector<valStruct> possibles;
};

class Brain {
private:
    std::vector<puzzle_cell_ptr> puzzleCells_;
    std::vector<global_cell_ptr> globalCells_;
    std::vector<io_ptr> brainPorts_;

    // holds previous solutions when we get stuck
    std::vector<solutionPath> solutionPaths_;

    bool firstRun_ = true;
    long numRuns_ = 0;

    // Creates Puzzle Cells and adds them to puzzleCells_
    void createPuzzleCells();

    // Connects the Puzzle Cells in each row via an h direction port
    void connectPuzzleRows();

    // Connects the Puzzle Cells in each column via a v direction port
    void connectPuzzleCols();

    // Creates Global Cells and adds them to globalCells_
    void createGlobalCells();

    //Connects all the Global Cells to their intended Puzzle Cells
    void connectGlobals();

    // Connects this Brain to the Puzzle Cells via a b direction port
    void connectBrainToPuzzleCells();

public:
    /**
    * Reset a sudoku brain to the below map.
    * C = Puzzle Cell
    * G = Global Cell (Connector cells between Puzzle Cells to allow global messages within a block)
    * h = Horizontal port connection
    * v = Vertical port connection
    * g = Global connection between Global Cells and neighboring Puzzle Cells
    *
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *
    *   v       v       v       v       v       v       v       v       v
    *
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *
    *   v       v       v       v       v       v       v       v       v
    *
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   v   G   v   G   v       v   G   v   G   v       v   G   v   G   v
    *     g   g   g   g           g   g   g   g           g   g   g   g
    *   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C   h   C
    */
    void reset();

    puzzle_cell_ptr getPuzzleCell(const uint row, const uint col);

    global_cell_ptr getGlobalCell(const uint row, const uint col);

    io_ptr getBrainPort(const uint row, const uint col);

    // Run until there's nothing else to do
    // Set debug to true to output state information as it runs
    int solve(bool debug = false);

    // Runs all the hubs for all the cells until we either solve the puzzle or
    // there are no more messages being generated
    bool run(bool debug = false);

    // set the value of a particular cell via a management message
    void setValue(const uint row, const uint col, const uint value);

    // Directly set the sole value and possible values, avoiding sending an "rm" message to all other cells
    void setValue(const uint row, const uint col, const uint value, const std::vector<uint> possibles);

    // Initialize the Puzzle Cells to the given values
    // Where a value is 1-9, and 0 represents a cell whose value hasn't been set
    void setValues(const vector<uint> vector);

    // Initialize the Puzzle Cells to the given solution
    void setValues(const vector <puzzleState> solution);

    // Remove the given value from the Puzzle Cell at the given row and column
    void removeValue(const uint row, const uint col, const uint value);

    // Get the values from the Puzzle Cell at the given row and column
    vector<uint> *getValues(const uint row, const uint col);

    void printValues();

    // Print the current calculated solution of the entire puzzle to cout
    // Will print values of 1-9, if that is the Puzzle Cell's sole value,
    // Otherwise, will print *
    void printSolution();

    // Print the number of each Puzzle Cell's received messages in
    // the format of a Sudoku puzzle grid
    void printNumMsgsRcvd();

    // Return true if all the Puzzle Cells contain a sole value
    bool isPuzzleSolved();

    std::vector<uint> getSolution();

    std::vector<puzzleState> getSolutionStruct();

    vector<valStruct> getPossibleSolutions();
};


#endif //SUDOKU_SOLVER_BRAIN_H
