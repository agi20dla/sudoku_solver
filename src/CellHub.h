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

class CellHub: public Hub {

public:
    virtual void run();
    CellHub();
    CellHub(const CellHub & other);

    CellHub & operator=(const CellHub & other);
};


#endif //SUDOKU_SOLVER_HUB_H
