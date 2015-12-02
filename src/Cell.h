//
// Created by jason on 11/23/15.
//

#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H

#include <boost/unordered_map.hpp>
#include <boost/uuid/uuid.hpp>
#include "common.h"

class Cell {
protected:
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > messages_;
    cell_hub_ptr cellHub_;

public:
    Cell();

    Cell(const Cell &other);

    void connect(shared_ptr<Cell> cell, const std::string &direction = "g");

    io_ptr connect(const std::string &direction = "g");

    io_ptr connect(io_ptr otherPort, const std::string &direction);

    virtual void run();

    io_ptr getMsgConnection(const std::string &direction = "g");

    ulong numMessagesOnQueue();
    ulong numMessagesSent();
    ulong numMessagesRcvd();
    ulong numConnections();

    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
