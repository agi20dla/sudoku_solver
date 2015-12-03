//
// Created by jason on 6/17/15.
//

#ifndef SUDOKU_SOLVER_HUB_H
#define SUDOKU_SOLVER_HUB_H


#include "IoMessage.h"
#include "ConcurrentQueue.h"
#include <boost/unordered_map.hpp>
#include "common.h"

class IoPort;

class Hub {
private:
    ConcurrentQueue<IoMessage> messageQueue_;

protected:
    std::vector<io_ptr> ioPorts_;
    boost::unordered_map<boost::uuids::uuid, uint> messageUUIDs;

    ulong messagesSent_;
    ulong messagesRcvd_;
    ulong numPorts_;


public:
    virtual void run();

    Hub();

    Hub(const Hub &other);

    bool tryPop(IoMessage &popped);

    void push(IoMessage ioMessage);

    void addIoPort(io_ptr ioPort);

    ulong numMessagesOnQueue();

    ulong numMessagesSent();

    ulong numMessagesRcvd();

    ulong numPorts();
};


#endif //SUDOKU_SOLVER_HUB_H
