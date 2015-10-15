//
// Created by jason on 6/17/15.
//

#ifndef SUDOKU_SOLVER_HUB_H
#define SUDOKU_SOLVER_HUB_H


#include "IoMessage.h"
#include "ConcurrentQueue.h"
#include <boost/unordered_map.hpp>

class IoPort;

class Hub {
private:
    std::shared_ptr<ConcurrentQueue<IoMessage>> messageQueue_;

protected:
    std::vector<std::shared_ptr<IoPort>> ioPorts_;

    std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsReceived_;

    ulong messagesSent_;
    ulong messagesRcvd_;
    ulong numPorts_;

public:
    virtual void run() = 0;

    Hub();
    Hub(const Hub & other);

    bool tryPop(IoMessage& ioMessage);
    void push(IoMessage ioMessage);

    std::vector<std::shared_ptr<IoPort>>& getIoPorts();

    void addIoPort(std::shared_ptr<IoPort> ioPort);

    ulong numMessagesOnQueue();

    ulong numMessagesSent();

    ulong numMessagesRcvd();

    ulong numPorts();

    Hub & operator=(const Hub & other);

};


#endif //SUDOKU_SOLVER_HUB_H
