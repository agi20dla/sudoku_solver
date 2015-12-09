//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_EXCEPTIONS_H
#define SUDOKU_SOLVER_EXCEPTIONS_H

//#include <boost/exception/all.hpp>
#include <exception>

struct port_already_connected : /*public boost::exception,*/ public std::exception {
    const char *what() const noexcept { return "Port already connected"; }
};

struct attempt_to_remove_sole_value : /*public boost::exception,*/ public std::exception {
    const char *what() const noexcept { return "Attempted to remove sole value"; }
};

struct bad_puzzle_file : /*public boost::exception,*/ public std::exception {
    const char *what() const noexcept { return "File is not a Sudoku Solver puzzle file"; }
};
#endif //SUDOKU_SOLVER_EXCEPTIONS_H
