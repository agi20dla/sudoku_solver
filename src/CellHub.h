//
// Created by jason on 5/22/15.
//

#ifndef SUDOKU_SOLVER_CELL_HUB_H
#define SUDOKU_SOLVER_CELL_HUB_H

#include <memory>

#include <boost/uuid/uuid.hpp>
#include <boost/unordered_map.hpp>

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "IoPort.h"
//#include "Cell.h"

class Cell;

class CellHub {
private:
    std::shared_ptr<ConcurrentQueue<msg_ptr> > messageQueue_;
    std::shared_ptr<std::vector<io_ptr> > ioPorts_;
    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > messageUUIDs_;

    ulong messagesSent_;
    ulong messagesRcvd_;
    ulong numPorts_;

    Cell *cell_;

public:
    CellHub();
    CellHub(const CellHub & other);

    void run();

    void setCell(Cell *cell);

    Cell *getCell() const;

    msg_ptr tryPop();

    void push(msg_ptr ioMessage);

    void addIoPort(io_ptr ioPort);

    void processCommand(const string command, const uint value);

    void broadcast(const string command, const uint value);

    ulong numMessagesOnQueue();

    ulong numMessagesSent();

    ulong numMessagesRcvd();

    ulong numPorts();

    CellHub &operator=(const CellHub &other);
};


#endif //SUDOKU_SOLVER_HUB_H
