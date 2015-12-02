//
// Created by jason on 5/22/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/IoPort.h"
#include "CellHub.h"
#include "PuzzleCell.h"

using namespace std;

CellHub::CellHub()
        : messageQueue_(make_shared<ConcurrentQueue<msg_ptr> >()), ioPorts_(make_shared<vector<io_ptr> >()),
          messageUUIDs_(make_shared<boost::unordered_map<boost::uuids::uuid, uint> >()), messagesSent_(0),
          messagesRcvd_(0), numPorts_(0), cell_(nullptr) { }

CellHub::CellHub(const CellHub &other)
        : messageQueue_(other.messageQueue_), ioPorts_(other.ioPorts_), messageUUIDs_(other.messageUUIDs_),
          messagesSent_(other.messagesSent_), messagesRcvd_(other.messagesRcvd_), numPorts_(0), cell_(other.cell_) {
}


void CellHub::addIoPort(io_ptr ioPort) {
    ioPorts_->push_back(ioPort);
    numPorts_++;
}

void CellHub::setCell(Cell *cell) {
    cell_ = cell;
}

Cell *CellHub::getCell() const {
    return cell_;
}

/**
 * Iterate through all the messages on the message queue and send to the IO ports
 */
void CellHub::run() {
    msg_ptr ioMessage;

    while ((ioMessage = tryPop())) {
        // save the command and the value
        string command = ioMessage->getMessage();
        uint value = ioMessage->getValue();

        // send message to ports
        boost::uuids::uuid rcvPortUuid = ioMessage->getRcvPortUuid();
        for (io_ptr ioPort : *ioPorts_) {
            if (rcvPortUuid != ioPort->getUuid()
                && ioPort->getDirection() == ioMessage->getDirection()) {
                if (ioPort->sendToExt(ioMessage)) {
                    ++messagesSent_;
                }
            }
        }

        // process command and value
        processCommand(command, value);
        // get message command and value before putting on next IoPort (at beginning of while)
        // instead of sending to mgthub, deal with it here.
//        sendMsgToMgt(ioMessage);
    }
}

///**
// * send out a message to the mgt hub to do stuff with the values
// */
//void CellHub::sendMsgToMgt(msg_ptr msg) {
//    for (io_ptr ioPort : ioPorts_) {
//        if (ioPort->getDirection() == "m") {
//            ioPort->sendToExt(msg);
//            break;
//        }
//    }
//}

CellHub &CellHub::operator=(const CellHub &other) {
    messageQueue_ = other.messageQueue_;
    ioPorts_ = other.ioPorts_;
    messageUUIDs_ = other.messageUUIDs_;
    messagesSent_ = other.messagesSent_;
    messagesRcvd_ = other.messagesRcvd_;
    numPorts_ = other.numPorts_;
    cell_ = other.cell_;
    return *this;
}

void CellHub::processCommand(const string command, const uint value) {
    if (command == "rm") {
        // remove this value as a possible solution
        (cell_->getPossibleValues()[value]) = 0;

        // if removing a value leaves only one possible value
        // then send a rm message to the cell for that value
        if (soleValue == 0) {
            uint testValue = 0;
            int numValues = 0;
            uint idx = 0;
            for (auto possible : *possible_values_) {
                // If we already have one value and we found another one,
                // then there's nothing to do
                if (*possible == 1 && testValue > 0) {
                    return;
                } else if (*possible == 1) {
                    testValue = idx;
                    numValues++;
                }
                idx++;
            }

            soleValue = testValue;
            broadcast("rm", soleValue);
        }

        // if we got here, then we only have one value, so
        // send out a broadcast message to remove that value as a possibility
    } else if (command == "set") {
        // reset all other values, ensuring that only this value is set,
        // then send g, h and, v messages to reset this value in all other
        // relevant cells
        uint idx = 0;
        for (auto possible : *possible_values_) {
            if (idx == value) {
                *possible = 1;
            } else {
                *possible = 0;
            }
            idx++;
        }
        broadcast("rm", value);
    }
}

/**
 * send out a message to the cell to do stuff
 */
void CellHub::broadcast(const string msg, const uint value) {
    msg_ptr globalMsg = make_shared<IoMessage>(string(msg), value, "g");
    msg_ptr hMsg = make_shared<IoMessage>(string(msg), value, "h");
    msg_ptr vMsg = make_shared<IoMessage>(string(msg), value, "v");

    for (io_ptr ioPort : *ioPorts_) {
        if (ioPort->getDirection() == "m") {
            ioPort->sendToExt(globalMsg);
            ioPort->sendToExt(hMsg);
            ioPort->sendToExt(vMsg);
            break;
        }
    }
}


ulong CellHub::numMessagesOnQueue()
{
    return messageQueue_->size();
}

ulong CellHub::numMessagesSent() {
    return messagesSent_;
}

ulong CellHub::numMessagesRcvd() {
    return messageQueue_->get_num_messages_rcvd();
}

ulong CellHub::numPorts() {
    return numPorts_;
}

msg_ptr CellHub::tryPop() {
    return messageQueue_->try_pop();
}

void CellHub::push(msg_ptr ioMessage) {
    messageQueue_->push(ioMessage);
}
