//
// Created by jason on 11/23/15.
//

#include "Cell.h"
#include "CellHub.h"

using namespace std;

Cell::Cell()
        : messages_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>()), cellHub_(make_shared<CellHub>())
{
//    cellHub_->setCell(shared_from_this());
}

Cell::Cell(const Cell &other)
{
    this->messages_ = other.messages_;
    this->cellHub_ = other.cellHub_;
}



/**
 * create a new port and send it to the other cell for a connection
 * The other cell will also create a new port, connect the ports, and
 * return it's port so we can connect to it.
 */
void Cell::connect(shared_ptr<Cell> otherCell, const string &direction) {
    io_ptr port = getMsgConnection(direction);

    if (otherCell) {
        io_ptr otherPort = otherCell->connect(port, direction);
        port->connect(otherPort);
    }
}


/**
 * create a management port connected to the messaging queue
 * send that port back so the caller can connect to us
 */
io_ptr Cell::connect(const std::string &direction) {
    io_ptr port = getMsgConnection(direction);
    return port;
}

void Cell::run() {
    cellHub_->run();
}

io_ptr Cell::connect(io_ptr otherPort, const string &direction)
{
    io_ptr port = getMsgConnection(direction);
    port->connect(otherPort);
    return port;
}

/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr Cell::getMsgConnection(const string &direction) {
    io_ptr port = std::make_shared<IoPort>(cellHub_, messages_, direction);
    cellHub_->addIoPort(port);
    return port;
}

ulong Cell::numMessagesOnQueue() {
    ulong numMsgs = cellHub_->numMessagesOnQueue();
    return numMsgs;
}

ulong Cell::numMessagesSent() {
    return cellHub_->numMessagesSent();
}

ulong Cell::numMessagesRcvd() {
    return cellHub_->numMessagesRcvd();
}

ulong Cell::numConnections() {
    return cellHub_->numPorts();
}

Cell &Cell::operator=(const Cell &other) {
    if (this == &other) {
        return *this;
    }
    this->cellHub_ = other.cellHub_;
    return *this;
}
