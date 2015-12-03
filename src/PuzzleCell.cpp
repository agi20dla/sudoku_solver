//
// Created by jason on 5/23/15.
//

#include "PuzzleCell.h"

using namespace std;

PuzzleCell::PuzzleCell()
        : soleValue_(0)
        , hub_(make_shared<CellHub>())
{
    hub_->setCell(this);
}

PuzzleCell::PuzzleCell(const PuzzleCell &other)
        : Cell(other)
        , soleValue_(other.soleValue_)
{
}

void PuzzleCell::run()
{
    hub_->run();
}



/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr PuzzleCell::getMsgConnection(const string &direction) {
    io_ptr port = std::make_shared<IoPort>(hub_, rcvdMsgUUIDs_, direction);
    hub_->addIoPort(port);
    return port;
}

ulong PuzzleCell::numMessagesOnQueue() {
    ulong numMsgs = hub_->numMessagesOnQueue();
    return numMsgs;
}

ulong PuzzleCell::numMessagesSent() {
    return hub_->numMessagesSent();
}

ulong PuzzleCell::numMessagesRcvd() {
    return hub_->numMessagesRcvd();
}

ulong PuzzleCell::numConnections() {
    return hub_->numPorts();
}

vector<uint> *PuzzleCell::getPossibleValues() {
    return &possibleValues_;
}

uint PuzzleCell::getSoleValue() {
    return soleValue_;
}

void PuzzleCell::setSoleValue(const uint soleValue) {
    soleValue_ = soleValue;
}
