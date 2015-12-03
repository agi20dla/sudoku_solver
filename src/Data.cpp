//
// Created by jason on 11/22/15.
//

#include "Data.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "Exceptions.h"
#include <boost/lexical_cast.hpp>

using namespace std;

Data::Data() {
}

void Data::init(const string filename) {
    ifstream myFile(filename);
    string line;
    string puzzle;
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            puzzle += line;
        }
        puzzle.erase(
                std::remove_if(puzzle.begin(), puzzle.end(),
                               [](char ch) {
                                   return std::isspace<char>(ch, std::locale::classic());
                               }), puzzle.end()
        );
        if (puzzle.length() != 81) {
            cerr << "File [" << filename << "] is not a Sudoku Solver puzzle file" << endl;
            throw bad_puzzle_file();
        } else {
            puzzle_str = puzzle;
        }
    } else {
        throw bad_puzzle_file();
    }

    for (auto c : puzzle_str) {
        int value;
        if ('.' == c) {
            value = 0;
        } else if (c > '0' && c <= '9') {
            value = boost::lexical_cast<uint>(c);
        } else {
            throw bad_puzzle_file();
        }
        puzzle_vec.push_back(value);
    }
}


void Data::init(const vector<uint> values)
{
    puzzle_vec = values;
}

string Data::getPuzzleStr() {
    return puzzle_str;
}

vector<uint> Data::getPuzzleVec() const {
    return puzzle_vec;
}


bool operator==(const Data &lhs, const Data &rhs) {
    return lhs.getPuzzleVec() == rhs.getPuzzleVec();
}

bool operator!=(const Data &lhs, const Data &rhs) {
    return !(lhs == rhs);
}
