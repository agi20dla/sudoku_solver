//
// Created by jason on 5/7/15.
//
#include <boost/uuid/uuid_generators.hpp>

#include "Exceptions.h"
#include "IoPort.h"
#include "Hub.h"

using namespace std;

boost::mutex IoPort::mutex_;

IoPort::IoPort(hub_ptr hub, std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed,
               string direction)
        : hub_(hub)
        , msgsProcessed_(msgsProcessed), myDirection_(direction)
{
    boost::uuids::random_generator generator;
    uuid_ = generator();
}

void IoPort::fwdToQueue(IoMessage ioMessage) {
    if (msgsProcessed_->find(ioMessage.getMsgUuid()) == msgsProcessed_->end()) {
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage.getMsgUuid(), 1);

        // lock the messages processed since we are not using a custom concurrent map.
        boost::mutex::scoped_lock lock(mutex_);
        msgsProcessed_->insert(newMsgUuid);
        lock.unlock();

        // store the uuid of this message in the message
        ioMessage.setRcvPortUuid(uuid_);
        ++numMessagesRecieved;

        // put it on the hub
        hub_->push(ioMessage);
        ++numMessagesForwarded;
    }
}


void IoPort::connect(io_ptr otherPort)
{
    if (!otherPort_) {
        otherPort_ = otherPort;
    } else {
        throw port_already_connected();
    }
}

/**
 * Send an IO Message to another port, if the message's direction
 * is the same as my direction.  If it's a management message
 * (direction == "m"), send it anyway
 */
bool IoPort::sendToExt(IoMessage ioMessage)
{
    bool sent = false;
    ++numMessagesRecieved;
    if (otherPort_ && (ioMessage.getDirection() == myDirection_ || myDirection_ == "m")) {
        otherPort_->fwdToQueue(ioMessage);
        sent = true;
        ++numMessagesSent;
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage.getMsgUuid(), 1);
        msgsProcessed_->insert(newMsgUuid);
    }
    return sent;
}

size_t IoPort::getNumMessagesForwarded() {
    return numMessagesForwarded;
}

size_t IoPort::getNumMessagesSent() {
    return numMessagesSent;
}

size_t IoPort::getNumMessagesRecieved(){
    return numMessagesRecieved;
}

boost::uuids::uuid IoPort::getUuid() {
    return uuid_;
}

const string IoPort::getDirection() {
    return myDirection_;
}
