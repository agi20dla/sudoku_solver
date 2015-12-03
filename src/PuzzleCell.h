//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_PUZZLECELL_H
#define SUDOKU_SOLVER_PUZZLECELL_H

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "Cell.h"
#include "common.h"

class PuzzleCell : public Cell {
private:
    // represents the possible value this cell can hold
    // initialized to all values (1-9) being possible
    std::vector<uint> possibleValues_{0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    uint soleValue_;
    cell_hub_ptr hub_;

public:
    PuzzleCell();

    PuzzleCell(const PuzzleCell &);

    void run() override;  // in GlobalCell

    io_ptr getMsgConnection(const std::string &direction) override;

    ulong numMessagesOnQueue() override;

    ulong numMessagesSent() override;

    ulong numMessagesRcvd() override;

    ulong numConnections() override;

    std::vector<uint> *getPossibleValues();

    uint getSoleValue();

    void setSoleValue(const uint soleValue);
};


#endif //SUDOKU_SOLVER_PUZZLECELL_H