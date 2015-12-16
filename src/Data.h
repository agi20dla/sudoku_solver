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

    void init(const std::string &filename);

    void init(const std::vector<uint> &values);

    std::string getDataAsString();

    std::vector<uint> getDataAsVector() const;

    friend bool operator==(const Data &lhs, const Data &rhs);

    friend bool operator!=(const Data &lhs, const Data &rhs);
};


#endif //SUDOKU_SOLVER_DATA_H
