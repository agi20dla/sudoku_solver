//
// Created by jason on 5/7/15.
//

#ifndef SUDOKU_SOLVER_IOPORT_H
#define SUDOKU_SOLVER_IOPORT_H

#include <boost/uuid/uuid.hpp>
#include <boost/thread/mutex.hpp>

#include <unordered_map>
#include <memory>
#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "common.h"

class IoPort
{
private:
    hub_ptr hub_;

    shared_ptr<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>> msgsProcessed_;

    io_ptr otherPort_;

    std::string myDirection_;

    boost::uuids::uuid uuid_;

    size_t numMessagesRecieved = 0;
    size_t numMsgsForwardedToHub = 0;
    size_t numMessagesSent = 0;

    static boost::mutex mutex_;

public:
    IoPort();


    // Sets the hub that this port is attached to
    void setHub(hub_ptr hub);

    // Adds the owning cell's map of received messages
    void addRcvdMsgMap(msg_map_ptr msgsProcessed);

    // Sets the direction for the messages this port will handle
    void setDirection(const std::string &direction);

    void init(hub_ptr hub, msg_map_ptr msgsProcessed, const std::string &direction);

    // Number of messages forwarded to the queue
    size_t getNumMsgsForwardedToHub();

    // Number of messages sent to this ports connected IoPort
    size_t getNumMessagesSent();

    // Number of messages recieved for forwarding or sending
    size_t getNumMessagesRecieved();

    // put a message on the queue
    void fwdToQueue(std::shared_ptr<IoMessage> ioMessage);

    // send a message to a connected IoPort
    bool sendToExt(std::shared_ptr<IoMessage> ioMessage);

    // Connect to another IoPort
    void connect(io_ptr otherPort);

    const string getDirection();

    boost::uuids::uuid getUuid();
};


#endif //SUDOKU_SOLVER_IOPORT_H
