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
protected:
//    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > mgtMessages_;

//    mgt_hub_ptr mgtHub_;

    // represents the possible value this cell can hold
    // initialized to all values (1-9) being possible
    std::vector<int_ptr> possibleValues_{std::make_shared<int>(0), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1)};
    uint soleValue;

public:
    PuzzleCell();

    PuzzleCell(const PuzzleCell &);

//    ~PuzzleCell();

    void run() override;  // in GlobalCell

//    io_ptr getMgtConnection(const std::string &direction = "m");

    std::vector<int_ptr> *getPossibleValues();

//    PuzzleCell & operator=(const PuzzleCell & other);
};


#endif //SUDOKU_SOLVER_PUZZLECELL_H
