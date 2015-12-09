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

    // Creates a new port and sends it to the other cell for a connection
    // The other cell will also create a new port, connect the ports, and
    // return it's port so we can connect to it.
    void connect(shared_ptr<Cell> cell, const std::string &direction);


    // Creates a port for this cell and returns it for the caller's use
    io_ptr connect(const std::string &direction);

    // Creates a port for this cell and connects the given port to it
    // Returns the newly created port for the caller's use
    io_ptr connect(io_ptr otherPort, const std::string &direction);

    // Run the derived cell's processing
    virtual void run() = 0;

    // Basic version of creating a port.  Just creates a port and sends it back
    // It is up to the implementation calsses to assign a hub to this port
    virtual io_ptr createPort(const std::string &direction);

    virtual ulong numMessagesOnHub() = 0;
    virtual ulong numMessagesSent() = 0;
    virtual ulong numMessagesRcvd() = 0;


    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
