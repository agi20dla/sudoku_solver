//
// Created by jason on 6/17/15.
//

#ifndef SUDOKU_SOLVER_HUB_H
#define SUDOKU_SOLVER_HUB_H


#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <memory>
#include "IoMessage.h"
#include "ConcurrentQueue.h"
#include "common.h"
#include "IoPort.h"

class Hub {
private:
    ConcurrentQueue messageQueue_;

protected:
    std::vector<io_ptr> ioPorts_;
    boost::uuids::uuid uuid_;
    std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>> messageUUIDs;

    ulong messagesSent_;
    ulong messagesRcvd_;
    ulong numPorts_;


public:
    virtual bool run();

    Hub();
    Hub(const Hub &other);

    std::shared_ptr<IoMessage> tryPop();

    void push(std::shared_ptr<IoMessage> ioMessage);

    void addIoPort(io_ptr ioPort);

    ulong getNumMsgsOnQueue();

    ulong getNumMsgsSent();

    ulong getNumMsgsRcvd();

    ulong getNumPorts();

    void clear();
};


#endif //SUDOKU_SOLVER_HUB_H
