//
// Created by jason on 5/7/15.
//
#include <boost/uuid/uuid_generators.hpp>

#include "Exceptions.h"
#include "IoPort.h"
#include "CellHub.h"
#include "Random.h"

using namespace std;

boost::mutex IoPort::mutex_;

IoPort::IoPort(hub_ptr hub, std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed,
               string direction)
        : hub_(hub), msgsProcessed_(msgsProcessed), myDirection_(direction)
{
    uuid_ = Random::getInstance().getNewUUID();
}

IoPort::IoPort(std::shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed, string direction)
        : hub_(nullptr), msgsProcessed_(msgsProcessed), myDirection_(direction) {
    uuid_ = Random::getInstance().getNewUUID();
}

void IoPort::fwdToQueue(IoMessage ioMessage) {
    if (msgsProcessed_->find(ioMessage.getUuid()) == msgsProcessed_->end()) {
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage.getUuid(), 1);

        // lock the messages processed since we are not using a custom concurrent map.
        boost::mutex::scoped_lock lock(mutex_);
        msgsProcessed_->insert(newMsgUuid);
        lock.unlock();

        ioMessage.setForwardingPortUUID(uuid_);
        ++numMessagesRecieved;

        // put it on the hub
        hub_->push(ioMessage);
        ++numMsgsForwardedToHub;
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
    if (otherPort_ && (ioMessage.getDirection() == myDirection_)) {
        otherPort_->fwdToQueue(ioMessage);
        sent = true;
        ++numMessagesSent;
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage.getUuid(), 1);
        msgsProcessed_->insert(newMsgUuid);
    }
    return sent;
}

size_t IoPort::getNumMsgsForwardedToHub() {
    return numMsgsForwardedToHub;
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

void IoPort::setHub(hub_ptr hub) {
    hub_ = hub;
}
