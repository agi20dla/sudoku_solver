//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_EXCEPTIONS_H
#define SUDOKU_SOLVER_EXCEPTIONS_H

#include <boost/exception/all.hpp>
#include <exception>

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info;

struct port_already_connected : public boost::exception, public std::exception {
    const char *what() const noexcept { return "Port already connected"; }
};

struct attempt_to_remove_sole_value : public boost::exception, public std::exception {
    const char *what() const noexcept { return "Message attempted to remove sole value"; }
};

struct array_out_of_bounds_exception : public boost::exception, public std::exception {
    const char *what() const noexcept { return "Array parameters out of bounds"; }
};
#endif //SUDOKU_SOLVER_EXCEPTIONS_H