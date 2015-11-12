//
// Created by jason on 5/23/15.
//

#ifndef SUDOKU_SOLVER_CELL_H
#define SUDOKU_SOLVER_CELL_H


#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "MgtHub.h"

class Cell {
private:
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived_;
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> mgtsReceived_;
    std::shared_ptr<CellHub> msgHub_;
    std::shared_ptr<MgtHub> mgtHub_;

    // represents the possible value this cell can hold
    // initialized to all values (1-9) being possible
    std::vector<uint> possibleValues_ {0,1,1,1,1,1,1,1,1,1};

    // This is the value of the cell.  It will be set to the last value left set
    // in the possibleValues structure
    uint value{0};
public:
    Cell();
    Cell(const Cell&);

    void run();

    shared_ptr<IoPort> getMsgConnection(const std::string& direction = "g");
    shared_ptr<IoPort> getMgtConnection(const std::string& direction = "m");

    void connect(shared_ptr<Cell> cell, const std::string& direction = "g");

    shared_ptr<IoPort> connect(shared_ptr<IoPort> otherPort, const std::string& direction);

    ulong numMessages();

    ulong numMessagesSent();
    ulong numMessagesRcvd();
    ulong numConnections();

    vector<uint> getValues();

    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
