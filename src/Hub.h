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
    std::shared_ptr<ConcurrentQueue<msg_ptr> > messageQueue_;

protected:
    std::vector<io_ptr> ioPorts_;

    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint> > msgsReceived_;

    ulong messagesSent_;
    ulong messagesRcvd_;
    ulong numPorts_;

public:
    virtual void run() = 0;

    Hub();
    Hub(const Hub & other);

    msg_ptr tryPop();

    void push(msg_ptr ioMessage);

    void addIoPort(io_ptr ioPort);

    ulong numMessagesOnQueue();

    ulong numMessagesSent();

    ulong numMessagesRcvd();

    ulong numPorts();

    Hub & operator=(const Hub & other);

};


#endif //SUDOKU_SOLVER_HUB_H
