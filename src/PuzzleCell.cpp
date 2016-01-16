//
// Created by jason on 5/23/15.
//

#include "PuzzleCell.h"

using namespace std;

PuzzleCell::PuzzleCell()
        : soleValue_(0)
{
}


PuzzleCell::~PuzzleCell()
{
    possibleValues_.clear();
    hub_.reset();
}

void PuzzleCell::init() {
    hub_ = make_shared<CellHub>();
    hub_->setCell(getPtr());
}

std::shared_ptr<PuzzleCell> PuzzleCell::getPtr() {
    return shared_from_this();
}

bool PuzzleCell::run()
{
    return hub_->run();
}


/**
 * create a port connected to the message queue
 * add that port to the message hub
 * send that port back to the calling cell so it can connect to us
 */
io_ptr PuzzleCell::createPort(const string &direction) {
    io_ptr port = Cell::createPort(direction);
    port->setHub(hub_);
    hub_->addIoPort(port);
    return port;
}

bool PuzzleCell::hubHasMessages() {
    return hub_->hasMessages();
}

ulong PuzzleCell::numMessagesOnHub() {
    ulong numMsgs = hub_->getNumMsgsOnQueue();
    return numMsgs;
}


ulong PuzzleCell::numMessagesSent() {
    return hub_->getNumMsgsSent();
}


ulong PuzzleCell::numMessagesRcvd() {
    return hub_->getNumMsgsRcvd();
}


//ulong PuzzleCell::numPortsToHub() {
//    return hub_->getNumPorts();
//}


vector<uint> *PuzzleCell::getPossibleValues() {
    return &possibleValues_;
}


uint PuzzleCell::getSoleValue() {
    return soleValue_;
}


void PuzzleCell::setSoleValue(const uint soleValue) {
    soleValue_ = soleValue;
    possibleValues_ = vector<uint>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    possibleValues_.at(soleValue) = 1;
    hub_->broadcast("rm", soleValue);
}

void PuzzleCell::setPossibles(const vector<uint> &possibles) {
    possibleValues_ = vector<uint>(possibles);
}

void PuzzleCell::reset(const uint soleValue, const vector<uint> possibles) {
    hub_->clear();
    soleValue_ = soleValue;
    possibleValues_ = std::vector<uint>{possibles};
}
