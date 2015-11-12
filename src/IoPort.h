//
// Created by jason on 5/7/15.
//

#ifndef SUDOKU_SOLVER_IOPORT_H
#define SUDOKU_SOLVER_IOPORT_H

#include <boost/uuid/uuid.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>

#include <memory>
#include "ConcurrentQueue.h"
#include "IoMessage.h"
//#include "Hub.h"

class Hub;

class IoPort
{
private:
    std::shared_ptr<Hub> hub_;

    // TODO: might have to worry about concurrency on this one
    shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed_;

    std::shared_ptr<IoPort> otherPort_;

    std::string myDirection_;

    boost::uuids::uuid uuid_;

    size_t numMessagesRecieved = 0;
    size_t numMessagesForwarded = 0;
    size_t numMessagesSent = 0;

    static boost::mutex mutex_;

public:
    IoPort(std::shared_ptr<Hub> hub, std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed, std::string direction);

    // Number of messages forwarded to the queue
    size_t getNumMessagesForwarded();

    // Number of messages sent to this ports connected IoPort
    size_t getNumMessagesSent();

    // Number of messages recieved for forwarding or sending
    size_t getNumMessagesRecieved();

    // put a message on the queue
    void fwdToQueue(IoMessage ioMessage);

    // send a message to a connected IoPort
    bool sendToExt(IoMessage ioMessage);

    // Connect to another IoPort
    void connect(std::shared_ptr<IoPort> otherPort);

    const string getDirection();

    boost::uuids::uuid getUuid();
};


#endif //SUDOKU_SOLVER_IOPORT_H
