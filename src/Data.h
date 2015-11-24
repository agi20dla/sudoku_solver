//
// Created by jason on 11/22/15.
//

#ifndef SUDOKU_SOLVER_DATA_H
#define SUDOKU_SOLVER_DATA_H

#include <string>
#include <vector>

class Data {

private:
    std::string puzzle_str;
    std::vector<uint> puzzle_vec;

public:
    Data();

    void init(const std::string filename);

    std::string getPuzzleStr();

    std::vector<uint> getPuzzleVec();

    void loadSolution(std::vector<uint> solution);
};


#endif //SUDOKU_SOLVER_DATA_H
