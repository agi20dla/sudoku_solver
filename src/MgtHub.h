//
// Created by jason on 6/17/15.
//

#ifndef SUDOKU_SOLVER_MGTHUB_H
#define SUDOKU_SOLVER_MGTHUB_H


#include "Hub.h"

class MgtHub: public Hub {
private:
    std::vector<string> split(const string &s, char delim);
    std::vector<string>& split(const string &s, char delim, vector<string> &elems);
    std::vector<uint>* possible_values_;

    void sendMsgToCell(string msg);

public:
    virtual void run();

    MgtHub();
    MgtHub(const MgtHub & other);

    void addPossibleValues(std::vector<uint>* possible_values);

    MgtHub & operator=(const MgtHub & other);
};


#endif //SUDOKU_SOLVER_MGTHUB_H
