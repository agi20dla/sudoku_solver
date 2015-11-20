//
// Created by jason on 11/20/15.
//

#ifndef SUDOKU_SOLVER_BASICCELL_H
#define SUDOKU_SOLVER_BASICCELL_H

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "CellHub.h"
#include "common.h"

class BasicCell {
private:
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > msgsReceived_;
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > mgtsReceived_;
    cell_hub_ptr msgHub_;

public:
    BasicCell();

    BasicCell(const BasicCell &);

    void run();

    io_ptr getMsgConnection(const std::string &direction = "g");

    void connect(cell_ptr cell, const std::string &direction = "g");

    ulong numMessages();

    ulong numMessagesSent();

    ulong numMessagesRcvd();

    ulong numConnections();

    BasicCell &operator=(const BasicCell &other);

};


#endif //SUDOKU_SOLVER_BASICCELL_H
