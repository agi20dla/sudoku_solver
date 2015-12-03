//
// Created by jason on 6/5/15.
//

#ifndef SUDOKU_SOLVER_BRAIN_H
#define SUDOKU_SOLVER_BRAIN_H

#include <vector>
#include "IoPort.h"
#include "PuzzleCell.h"
#include "common.h"

class Brain {
private:
    // cellMap is a one dimensional array representing a two dimensional array.
    // its indexed by row * 9 + col.
    std::vector<puzzle_cell_ptr> puzzleCells_;
    std::vector<global_cell_ptr> globalCells_;
    std::vector<io_ptr> brainPorts_;


    void createPuzzleCells();

    void connectPuzzleRows();

    void connectPuzzleCols();

    void createGlobalCells();
    void connectGlobals();

    void connectBrainToPuzzleCells();

public:
//    Brain();

    void printNumMsgsRcvd();

    void printConnections();

    void reset();

    void pushCell(puzzle_cell_ptr cell);

    puzzle_cell_ptr getCell(const uint row, const uint col);

    puzzle_cell_ptr getCell(const uint index);

    void pushGlobal(global_cell_ptr cell);

    global_cell_ptr getGlobal(const uint row, const uint col);

    global_cell_ptr getGlobal(const uint index);

    io_ptr getMgtPort(const uint row, const uint col);

    void run(bool debug = false);

    // set the value of a particular cell via a management message
    void setValue(const uint row, const uint col, const uint value);

    // remove a particular value of a particular cell via a managment message
    void removeValue(const uint row, const uint col, const uint value);

    vector<uint> *getValues(const uint row, const uint col);

    void printValues();

    void printSolution();

    void initialize(const vector<uint> vector);

    std::vector<uint> getSolution();
};


#endif //SUDOKU_SOLVER_BRAIN_H
