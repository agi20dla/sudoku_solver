//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_PUZZLECELL_H
#define SUDOKU_SOLVER_PUZZLECELL_H

#include <memory>
#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "Cell.h"
#include "common.h"

class PuzzleCell : public Cell, public std::enable_shared_from_this<PuzzleCell> {
private:
    // represents the possible values this cell can hold
    // initialized to all values (1-9) being possible
    // the 0 value is not used in this context
    std::vector<uint> possibleValues_{0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    uint soleValue_;
    cell_hub_ptr hub_;

public:
    PuzzleCell();

    PuzzleCell(const PuzzleCell &other) = delete;

    virtual ~PuzzleCell();

    void init();

    std::shared_ptr<PuzzleCell> getPtr();

    bool run() override;  // in GlobalCell

    io_ptr createPort(const std::string &direction) override;

    bool hubHasMessages() override;
    ulong numMessagesOnHub() override;
    ulong numMessagesSent() override;
    ulong numMessagesRcvd() override;

//    ulong numPortsToHub() override;

    void reset(const uint soleValue, const vector<uint> possibles);

    std::vector<uint> *getPossibleValues();

    void setPossibles(const std::vector<uint> &possibles);

    uint getSoleValue();

    void setSoleValue(const uint soleValue);
};


#endif //SUDOKU_SOLVER_PUZZLECELL_H
