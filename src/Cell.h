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
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > rcvdMsgUUIDs_;

public:
    Cell();

    Cell(const Cell &other);

    void connect(shared_ptr<Cell> cell, const std::string &direction);
    io_ptr connect(const std::string &direction);
    io_ptr connect(io_ptr otherPort, const std::string &direction);

    virtual void run() = 0;

    virtual io_ptr getMsgConnection(const std::string &direction) = 0;

    virtual ulong numMessagesOnQueue() = 0;
    virtual ulong numMessagesSent() = 0;
    virtual ulong numMessagesRcvd() = 0;
    virtual ulong numConnections() = 0;

    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
