//
// Created by jason on 11/20/15.
//

#ifndef SUDOKU_SOLVER_GLOBALCELL_H
#define SUDOKU_SOLVER_GLOBALCELL_H

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "Cell.h"
#include "common.h"

class GlobalCell : public Cell {

public:
    GlobalCell();

    GlobalCell(const GlobalCell &other);

    GlobalCell &operator=(const GlobalCell &other);

};


#endif //SUDOKU_SOLVER_GLOBALCELL_H
