//
// Created by jason on 6/17/15.
//

#ifndef SUDOKU_SOLVER_MGTHUB_H
#define SUDOKU_SOLVER_MGTHUB_H


#include "Hub.h"

class MgtHub: public Hub {
private:
    std::vector<uint>* possible_values_;

    void broadcast(string msg, uint value);

    uint soleValue = 0;

public:
    virtual void run() override;

    MgtHub();
    MgtHub(const MgtHub & other);

    void addPossibleValues(std::vector<uint>* possible_values);

    MgtHub & operator=(const MgtHub & other);
};


#endif //SUDOKU_SOLVER_MGTHUB_H
