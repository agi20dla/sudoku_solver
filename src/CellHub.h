//
// Created by jason on 5/22/15.
//

#ifndef SUDOKU_SOLVER_CELL_HUB_H
#define SUDOKU_SOLVER_CELL_HUB_H

#include <memory>

#include <boost/uuid/uuid.hpp>
#include <boost/unordered_map.hpp>

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "IoPort.h"
#include "Hub.h"
//#include "Cell.h"

class Cell;

class CellHub : public Hub {
private:

    PuzzleCell *cell_;

public:
    CellHub();
    CellHub(const CellHub & other);

    void run() override;

    void setCell(PuzzleCell *cell);

    Cell *getCell() const;

    void processCommand(const string& command, const uint value);

    void broadcast(const string command, const uint value);
};


#endif //SUDOKU_SOLVER_HUB_H
