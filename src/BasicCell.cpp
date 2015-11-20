//
// Created by jason on 11/20/15.
//

#include "BasicCell.h"
#include "Cell.h"

using namespace std;

BasicCell::BasicCell()
        : msgsReceived_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>()),
          mgtsReceived_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>()), msgHub_(make_shared<CellHub>())
//        , mgtHub_(make_shared<MgtHub>())
{
//    io_ptr mgtPort = std::make_shared<IoPort>(mgtHub_, mgtsReceived_, "m");
//    mgtHub_->addIoPort(mgtPort);
    io_ptr msgPort = std::make_shared<IoPort>(msgHub_, msgsReceived_, "m");
    msgHub_->addIoPort(msgPort);
//    mgtPort->connect(msgPort);
//    msgPort->connect(mgtPort);

//    mgtHub_->addPossibleValues(&possibleValues_);
}

BasicCell::BasicCell(const BasicCell &cell) {
    this->msgsReceived_ = cell.msgsReceived_;
    this->msgHub_ = cell.msgHub_;
//    this->mgtHub_ = cell.mgtHub_;
}

/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr BasicCell::getMsgConnection(const string &direction) {
    io_ptr port = std::make_shared<IoPort>(msgHub_, msgsReceived_, direction);
    msgHub_->addIoPort(port);
    return port;
}

///**
// * create a management port connected to the messaging queue
// * send that port back so the caller can connect to us
// */
//io_ptr BasicCell::getMgtConnection(const std::string &direction)
//{
//    io_ptr port = std::make_shared<IoPort>(mgtHub_, mgtsReceived_, direction);
//    mgtHub_->addIoPort(port);
//    return port;
//}

void BasicCell::run() {
//    mgtHub_->run();
    msgHub_->run();
}

/**
 * create a new port and send it to the other cell for a connection
 * The other cell will also create a new port, connect the ports, and
 * return it's port so we can connect to it.
 */
void BasicCell::connect(cell_ptr otherCell, const string &direction) {
    io_ptr port = getMsgConnection(direction);
    if (otherCell) {
        io_ptr otherPort = otherCell->connect(port, direction);
        port->connect(otherPort);
    }
}

ulong BasicCell::numMessages() {
    ulong numMsgs = msgHub_->numMessagesOnQueue();
//    ulong numMgts = mgtHub_->numMessagesOnQueue();
    return numMsgs;
}

ulong BasicCell::numMessagesSent() {
    return msgHub_->numMessagesSent();
}

ulong BasicCell::numMessagesRcvd() {
    return msgHub_->numMessagesRcvd();
}

BasicCell &BasicCell::operator=(const BasicCell &other) {
    if (this == &other) {
        return *this;
    }
    this->msgHub_ = other.msgHub_;
//    this->mgtHub_ = other.mgtHub_;
    return *this;
}

ulong BasicCell::numConnections() {
    return msgHub_->numPorts();
}

//vector<uint> BasicCell::getValues() {
//    return possibleValues_;
//}
