//
// Created by jason on 5/7/15.
//
#include <boost/uuid/uuid_generators.hpp>

#include "Exceptions.h"
#include "IoPort.h"
#include "Hub.h"

using namespace std;

boost::mutex IoPort::mutex_;

IoPort::IoPort(shared_ptr<Hub> hub, shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed, string direction)
        : hub_(hub)
        , msgsProcessed_(msgsProcessed)
        , otherPort_(nullptr)
        , direction_(direction)
{
    boost::uuids::random_generator generator;
    uuid_ = generator();
}

IoPort::IoPort(shared_ptr<Hub> hub, shared_ptr<boost::unordered_map<boost::uuids::uuid, uint>> msgsProcessed)
        : hub_(hub)
        , msgsProcessed_(msgsProcessed)
        , otherPort_(nullptr)
        , direction_("g")
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


void IoPort::connect(shared_ptr<IoPort> otherPort)
{
    if (!otherPort_) {
        otherPort_ = otherPort;
    } else {
        throw port_connection_failed{};
    }
}


bool IoPort::sendToExt(IoMessage ioMessage)
{
    bool sent = false;
    ++numMessagesRecieved;
    if (otherPort_ && ioMessage.getDirection() == direction_) {
        otherPort_->fwdToQueue(ioMessage);
        sent = true;
        ++numMessagesSent;
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
    return direction_;
}
