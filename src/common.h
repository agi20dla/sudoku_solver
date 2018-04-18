//
// Created by jason on 11/18/15.
//

#ifndef SUDOKU_SOLVER_COMMON_H
#define SUDOKU_SOLVER_COMMON_H

//#include <boost/functional/hash.hpp>
//#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>
using namespace std;

class PuzzleCell;

class IoPort;

class Hub;

class CellHub;

class Cell;

class IoMessage;

class GlobalCell;

typedef std::shared_ptr<PuzzleCell> puzzle_cell_ptr;
typedef std::shared_ptr<GlobalCell> global_cell_ptr;
typedef std::shared_ptr<IoPort> io_ptr;
typedef std::shared_ptr<Hub> hub_ptr;
typedef std::shared_ptr<CellHub> cell_hub_ptr;
typedef std::shared_ptr<IoMessage> io_msg_ptr;



struct CellState
{
    uint soleValue;
    vector<uint> possibles;

    bool operator==(const CellState &other ) const {
        if (soleValue != other.soleValue) {
            return false;
        }
        for (uint idx = 0; idx < possibles.size(); idx++) {
            if (possibles.at(idx) != other.possibles.at(idx)) {
                return false;
            }
        }
        return true;
    }
};

struct CellValue {
    uint row;
    uint col;
    uint val;
};

struct SolutionPath {
    vector<CellState> states;
    vector<CellValue> possibles;

    bool operator==(const SolutionPath &other) const {
        for (uint idx = 0; idx < states.size(); idx++) {
            if (!(states.at(idx) == other.states.at(idx))) {
                return false;
            }
        }
        return true;
    }

    friend bool operator< (const SolutionPath& lhs, const SolutionPath& rhs);
    friend bool operator> (const SolutionPath& lhs, const SolutionPath& rhs);
    friend bool operator<= (const SolutionPath& lhs, const SolutionPath& rhs);
    friend bool operator>= (const SolutionPath& lhs, const SolutionPath& rhs);
};

struct StateHasher
{
    std::size_t operator()(const SolutionPath &sp) const {
        using boost::hash_value;
        using boost::hash_combine;

        std::size_t seed = 0;

        for (auto pz : sp.states) {
            hash_combine(seed, hash_value(pz.soleValue));
            for (auto p : pz.possibles) {
                hash_combine(seed, p);
            }
        }
        return seed;
    }
};

#endif //SUDOKU_SOLVER_COMMON_H
