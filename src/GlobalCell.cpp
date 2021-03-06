//
// Created by jason on 11/20/15.
//

#include "GlobalCell.h"

using namespace std;

GlobalCell::GlobalCell()
: hub_(make_shared<Hub>())
{ }

GlobalCell::GlobalCell(const GlobalCell &other)
        : Cell(other)
        , hub_(other.hub_)
{ }

GlobalCell::~GlobalCell()
{
    hub_.reset();
}


bool GlobalCell::run() {
    return hub_->run();
}


io_ptr GlobalCell::createPort(const string &direction) {
    io_ptr port = Cell::createPort(direction);
    port->setHub(hub_);
    hub_->addIoPort(port);
    return port;
}

bool GlobalCell::hubHasMessages() {
    return hub_->hasMessages();
}

ulong GlobalCell::numMessagesOnHub() {
    ulong numMsgs = hub_->getNumMsgsOnQueue();
    return numMsgs;
}

ulong GlobalCell::numMessagesSent() {
    return hub_->getNumMsgsSent();
}

ulong GlobalCell::numMessagesRcvd() {
    return hub_->getNumMsgsRcvd();
}

//ulong GlobalCell::numPortsToHub() {
//    return hub_->getNumPorts();
//}

GlobalCell &GlobalCell::operator=(const GlobalCell &other) {
    if (this == &other) {
        return *this;
    }
    this->hub_ = other.hub_;
    return *this;
}
