//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H


#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "MgtHub.h"
#include "common.h"

class Cell {
private:
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > msgsReceived_;
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > mgtsReceived_;
    cell_hub_ptr msgHub_;
    mgt_hub_ptr mgtHub_;

    // represents the possible value this cell can hold
    // initialized to all values (1-9) being possible
    std::vector<uint> possibleValues_ {0,1,1,1,1,1,1,1,1,1};

    // The sole value of this cell, if found
    uint soleValue = 0;

    // This is the value of the cell.  It will be set to the last value left set
    // in the possibleValues structure
    uint value{0};
public:
    Cell();
    Cell(const Cell&);

    void run();

    io_ptr getMsgConnection(const std::string &direction = "g");

    io_ptr getMgtConnection(const std::string &direction = "m");

    void connect(cell_ptr cell, const std::string &direction = "g");

    io_ptr connect(io_ptr otherPort, const std::string &direction);

    ulong numMessages();

    ulong numMessagesSent();
    ulong numMessagesRcvd();
    ulong numConnections();

    vector<uint> getValues();

    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
