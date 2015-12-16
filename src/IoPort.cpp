//
// Created by jason on 5/7/15.
//
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include "Exceptions.h"
#include "IoPort.h"
#include "CellHub.h"
#include "Random.h"

using namespace std;

//boost::mutex IoPort::mutex_;

IoPort::IoPort() {
    uuid_ = Random::getInstance().getNewUUID();
}



void IoPort::fwdToQueue(std::shared_ptr<IoMessage> ioMessage) {
    if (msgsProcessed_->find(ioMessage->getUuid()) == msgsProcessed_->end()) {
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage->getUuid(), 1);

//        // lock the messages processed since we are not using a custom concurrent map.
//        boost::mutex::scoped_lock lock(mutex_);
        msgsProcessed_->insert(newMsgUuid);
//        lock.unlock();

        ioMessage->setForwardingPortUUID(uuid_);
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
    ++numMessagesRecieved;
    if (otherPort_ && (ioMessage->getDirection() == myDirection_)) {
        otherPort_->fwdToQueue(ioMessage);
        sent = true;
        ++numMessagesSent;
        std::pair<boost::uuids::uuid, uint> newMsgUuid(ioMessage->getUuid(), 1);
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

void IoPort::addRcvdMsgMap(
        msg_map_ptr msgsProcessed) {
    msgsProcessed_ = msgsProcessed;
}

void IoPort::setDirection(const std::string &direction) {
    myDirection_ = direction;
}

void IoPort::init(hub_ptr hub, msg_map_ptr msgsProcessed, const std::string &direction) {
    hub_ = hub;
    msgsProcessed_ = msgsProcessed;
    myDirection_ = direction;

}
