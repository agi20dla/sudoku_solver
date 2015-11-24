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
    std::vector<int_ptr> possibleValues_{std::make_shared<int>(0), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1), std::make_shared<int>(1), std::make_shared<int>(1),
                                         std::make_shared<int>(1)};

public:
    Cell();
    Cell(const Cell&);

    ~Cell();

    void run();

    io_ptr getMsgConnection(const std::string &direction = "g");

    io_ptr getMgtConnection(const std::string &direction = "m");

    void connect(cell_ptr cell, const std::string &direction = "g");

    io_ptr connect(io_ptr otherPort, const std::string &direction);

    ulong numMessages();

    ulong numMessagesSent();
    ulong numMessagesRcvd();
    ulong numConnections();

    vector<int_ptr> *getValues();

    Cell& operator=(const Cell& other);
};


#endif //SUDOKU_SOLVER_CELL_H
