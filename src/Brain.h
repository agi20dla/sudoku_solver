//
// Created by jason on 6/5/15.
//

#ifndef SUDOKU_SOLVER_BRAIN_H
#define SUDOKU_SOLVER_BRAIN_H

#include <vector>
#include "IoPort.h"
#include "Cell.h"
#include "common.h"

class Brain {
private:
    // cellMap is a one dimensional array representing a two dimensional array.
    // its indexed by row * 9 + col.
    std::vector<cell_ptr> cellMap_;
    std::vector<basic_cell_ptr> globalMap_;
    std::vector<io_ptr> mgtPortMap_;


    void createCellMap();
    void connectRowCells();
    void connectColumnCells();

    void createGlobalMap();
    void connectGlobals();

    void connectBrainToCells();

public:
//    Brain();

    void printMessagesRcvd();

    void printConnections();

    void reset();

    void pushCell(cell_ptr cell);

    cell_ptr getCell(const uint row, const uint col);

    cell_ptr getCell(const uint index);

    void pushGlobal(basic_cell_ptr cell);

    basic_cell_ptr getGlobal(const uint row, const uint col);

    basic_cell_ptr getGlobal(const uint index);

    io_ptr getMgtPort(const uint row, const uint col);

    vector<uint> run(bool debug = false);

    // set the value of a particular cell via a management message
    void setValue(const uint row, const uint col, const uint value);

    // remove a particular value of a particular cell via a managment message
    void removeValue(const uint row, const uint col, const uint value);

    vector<int_ptr> *getValues(const uint row, const uint col);

    void printValues();

    void printSolution();

    void initialize(const vector<uint> vector);
};


#endif //SUDOKU_SOLVER_BRAIN_H
