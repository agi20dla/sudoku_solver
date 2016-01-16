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
    hub_ptr hub_;

public:
    GlobalCell();
    GlobalCell(const GlobalCell &other);
    virtual ~GlobalCell();

    bool run() override;

    io_ptr createPort(const std::string &direction) override;

    bool hubHasMessages() override;
    ulong numMessagesOnHub() override;
    ulong numMessagesSent() override ;
    ulong numMessagesRcvd() override;

    GlobalCell &operator=(const GlobalCell &other);
};


#endif //SUDOKU_SOLVER_GLOBALCELL_H
