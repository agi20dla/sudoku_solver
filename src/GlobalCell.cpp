//
// Created by jason on 11/20/15.
//

#include "GlobalCell.h"

using namespace std;

GlobalCell::GlobalCell()
: hub_(make_shared<Hub>())
{
}

GlobalCell::GlobalCell(const GlobalCell &other)
        : Cell(other)
        , hub_(other.hub_)
{ }

void GlobalCell::run() {
    hub_->run();
}



/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr GlobalCell::getMsgConnection(const string &direction) {
    io_ptr port = std::make_shared<IoPort>(hub_, rcvdMsgUUIDs_, direction);
    hub_->addIoPort(port);
    return port;
}

ulong GlobalCell::numMessagesOnQueue() {
    ulong numMsgs = hub_->numMessagesOnQueue();
    return numMsgs;
}

ulong GlobalCell::numMessagesSent() {
    return hub_->numMessagesSent();
}

ulong GlobalCell::numMessagesRcvd() {
    return hub_->numMessagesRcvd();
}

ulong GlobalCell::numConnections() {
    return hub_->numPorts();
}

GlobalCell &GlobalCell::operator=(const GlobalCell &other) {
    if (this == &other) {
        return *this;
    }
    this->hub_ = other.hub_;
    return *this;
}
