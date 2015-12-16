//
// Created by jason on 5/7/15.
//
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include "Exceptions.h"
#include "IoPort.h"
#include "CellHub.h"

using namespace std;

//boost::mutex IoPort::mutex_;

IoPort::IoPort() { }

void IoPort::fwdToQueue(std::shared_ptr<IoMessage> ioMessage) {
    if (!ioMessage->getHubUUID(hub_->getUUID())) {
        ioMessage->addHubUUID(hub_->getUUID());
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


bool IoPort::sendToExt(std::shared_ptr<IoMessage> ioMessage)
{
    bool sent = false;
    if (otherPort_ && (ioMessage->getDirection() == myDirection_) && !ioMessage->getHubUUID(otherPort_->getHubUuid())) {
        otherPort_->fwdToQueue(ioMessage);
        sent = true;
        ++numMessagesSent;
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


const string IoPort::getDirection() {
    return myDirection_;
}

void IoPort::setHub(hub_ptr hub) {
    hub_ = hub;
}

void IoPort::setDirection(const std::string &direction) {
    myDirection_ = direction;
}

void IoPort::init(hub_ptr hub, const std::string &direction) {
    hub_ = hub;
    myDirection_ = direction;

}

boost::uuids::uuid IoPort::getHubUuid() {
    return hub_->getUUID();
}
