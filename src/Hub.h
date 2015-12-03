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

    ulong getNumMsgsOnQueue();

    ulong getNumMsgsSent();

    ulong getNumMsgsRcvd();

    ulong getNumPorts();
};


#endif //SUDOKU_SOLVER_HUB_H
