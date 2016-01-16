//
// Created by jason on 11/23/15.
//

#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H

#include <boost/uuid/uuid.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <boost/functional/hash.hpp>
#include "common.h"

class Cell {
protected:
    std::shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> rcvdMsgUUIDs_;

public:
    Cell();

    Cell(const Cell &other);

    virtual ~Cell();

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
    virtual bool run() = 0;

    // Basic version of creating a port.  Just creates a port and sends it back
    // It is up to the implementation calsses to assign a hub to this port
    virtual io_ptr createPort(const std::string &direction);

    virtual bool hubHasMessages() = 0;
    virtual ulong numMessagesOnHub() = 0;
    virtual ulong numMessagesSent() = 0;
    virtual ulong numMessagesRcvd() = 0;


    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
