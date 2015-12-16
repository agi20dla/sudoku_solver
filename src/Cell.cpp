//
// Created by jason on 11/23/15.
//

#include "Cell.h"
#include "CellHub.h"

using namespace std;

Cell::Cell()
        : rcvdMsgUUIDs_(
        make_shared<std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>>>()) { }

Cell::Cell(const Cell &other)
        : rcvdMsgUUIDs_(other.rcvdMsgUUIDs_) { }

void Cell::connect(shared_ptr<Cell> otherCell, const string &direction) {
    io_ptr port = createPort(direction);

    if (otherCell) {
        io_ptr otherPort = otherCell->connect(port, direction);
        port->connect(otherPort);
    }
}


Cell::~Cell() {
    rcvdMsgUUIDs_->clear();
    rcvdMsgUUIDs_.reset();
}


io_ptr Cell::connect(const std::string &direction) {
    io_ptr port = createPort(direction);
    return port;
}


io_ptr Cell::connect(io_ptr otherPort, const string &direction)
{
    io_ptr port = createPort(direction);
    port->connect(otherPort);
    return port;
}


Cell &Cell::operator=(const Cell &other) {
    if (this == &other) {
        return *this;
    }
    return *this;
}


/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr Cell::createPort(const std::string &direction) {
    io_ptr port = std::make_shared<IoPort>();
    port->setDirection(direction);
    port->addRcvdMsgMap(rcvdMsgUUIDs_);
    return port;
}
