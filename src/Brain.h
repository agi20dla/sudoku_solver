//
// Created by jason on 6/5/15.
//

#ifndef SUDOKU_SOLVER_BRAIN_H
#define SUDOKU_SOLVER_BRAIN_H

#include <vector>
#include "IoPort.h"
#include "Cell.h"

class Brain {
private:
    // cellMap is a one dimensional array representing a two dimensional array.
    // its indexed by row * 9 + col.
    std::vector<std::shared_ptr<Cell>> cellMap_;
    std::vector<std::shared_ptr<Cell>> globalMap_;
    std::vector<std::shared_ptr<IoPort>> mgtPortMap_;


    void createCellMap();
    void connectRowCells();
    void connectColumnCells();

    void createGlobalMap();
    void connectGlobals();

    void connectBrainToCells();

    void printMessages();
//    void printConnections();

public:
//    Brain();

    void initialize();
    void pushCell(std::shared_ptr<Cell> cell);
    std::shared_ptr<Cell> getCell(const uint row, const uint col);

    void pushGlobal(std::shared_ptr<Cell> cell);
    std::shared_ptr<Cell> getGlobal(const uint row, const uint col);

    std::shared_ptr<IoPort> getMgtPort(const uint row, const uint col);

    void run();

    // set the value of a particular cell via a management message
    void setValue(const uint row, const uint col, const uint value);

    // remove a particular value of a particular cell via a managment message
    void removeValue(const uint row, const uint col, const uint value);

    std::shared_ptr<vector<uint>> getValues(const uint row, const uint col);
};


#endif //SUDOKU_SOLVER_BRAIN_H
