//
// Created by jason on 12/11/15.
//

#ifndef SUDOKU_SOLVER_RANDOM_H
#define SUDOKU_SOLVER_RANDOM_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

class Random {
public:
    static Random &getInstance() {
        static Random instance;
        return instance;
    }

private:
    boost::mt19937 ran;
    boost::uuids::random_generator random_generator;

    Random() {
        random_generator = boost::uuids::basic_random_generator<boost::mt19937>(ran);
    };

public:
    Random(Random const &) = delete;

    void operator=(Random const &) = delete;

    boost::uuids::uuid getNewUUID() {
        return random_generator();
    }
};

#endif //SUDOKU_SOLVER_RANDOM_H
