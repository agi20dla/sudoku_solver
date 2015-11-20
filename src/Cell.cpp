//
// Created by jason on 5/23/15.
//

#include "Cell.h"

using namespace std;

Cell::Cell()
        : msgsReceived_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>()),
          mgtsReceived_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>())
        , msgHub_(make_shared<CellHub>())
        , mgtHub_(make_shared<MgtHub>())
{
    io_ptr mgtPort = std::make_shared<IoPort>(mgtHub_, mgtsReceived_, "m");
    mgtHub_->addIoPort(mgtPort);
    io_ptr msgPort = std::make_shared<IoPort>(msgHub_, msgsReceived_, "m");
    msgHub_->addIoPort(msgPort);
    mgtPort->connect(msgPort);
    msgPort->connect(mgtPort);

    mgtHub_->addPossibleValues(&possibleValues_);
}

Cell::Cell(const Cell &cell) {
    this->msgsReceived_ = cell.msgsReceived_;
    this->msgHub_ = cell.msgHub_;
    this->mgtHub_ = cell.mgtHub_;
}

Cell::~Cell() {
    for (auto value : possibleValues_) {
        delete value;
    }
}

/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr Cell::getMsgConnection(const string &direction)
{
    io_ptr port = std::make_shared<IoPort>(msgHub_, msgsReceived_, direction);
    msgHub_->addIoPort(port);
    return port;
}

/**
 * create a management port connected to the messaging queue
 * send that port back so the caller can connect to us
 */
io_ptr Cell::getMgtConnection(const std::string &direction)
{
    io_ptr port = std::make_shared<IoPort>(mgtHub_, mgtsReceived_, direction);
    mgtHub_->addIoPort(port);
    return port;
}

void Cell::run()
{
    mgtHub_->run();
    msgHub_->run();
}

/**
 * create a new port and send it to the other cell for a connection
 * The other cell will also create a new port, connect the ports, and
 * return it's port so we can connect to it.
 */
void Cell::connect(cell_ptr otherCell, const string &direction)
{
    io_ptr port = getMsgConnection(direction);
    if (otherCell) {
        io_ptr otherPort = otherCell->connect(port, direction);
        port->connect(otherPort);
    }
}


io_ptr Cell::connect(io_ptr otherPort, const string &direction)
{
    io_ptr port = getMsgConnection(direction);
    port->connect(otherPort);
    return port;
}

ulong Cell::numMessages() {
    ulong numMsgs = msgHub_->numMessagesOnQueue();
    ulong numMgts = mgtHub_->numMessagesOnQueue();
    return numMgts + numMsgs;
}

ulong Cell::numMessagesSent() {
    return msgHub_->numMessagesSent();
}

ulong Cell::numMessagesRcvd() {
    return msgHub_->numMessagesRcvd();
}

Cell &Cell::operator=(const Cell &other) {
    if (this == &other) {
        return *this;
    }
    this->msgHub_ = other.msgHub_;
    this->mgtHub_ = other.mgtHub_;
    return *this;
}

ulong Cell::numConnections() {
    return msgHub_->numPorts();
}

vector<uint *> *Cell::getValues() {
    return &possibleValues_;
}
