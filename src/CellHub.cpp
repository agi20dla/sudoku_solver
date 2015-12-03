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
        : cell_(nullptr)
{ }

CellHub::CellHub(const CellHub &other)
        : cell_(other.cell_)
{ }


void CellHub::setCell(PuzzleCell *cell) {
    cell_ = cell;
}

void CellHub::run() {
    IoMessage ioMessage;

    while (tryPop(ioMessage)) {
        // save the command and the value
        string command = ioMessage.getCommand();
        uint value = ioMessage.getValue();
        // process command and value
        processCommand(command, value);

        // send message to ports
        boost::uuids::uuid fwdPortUuid = ioMessage.getForwardingPortUUID();
        for (io_ptr ioPort : ioPorts_) {
            if (fwdPortUuid != ioPort->getUuid()
                && ioPort->getDirection() == ioMessage.getDirection()) {
                if (ioPort->sendToExt(ioMessage)) {
                    ++messagesSent_;
                }
            }
        }
    }
}


void CellHub::processCommand(const string& command, const uint value) {
    if (command == "rm") {
        // remove this value as a possible solution
        (*cell_->getPossibleValues())[value] = 0;

        // if removing a value leaves only one possible value
        // then send a rm message to the cell for that value
        if (cell_->getSoleValue() == 0) {
            uint testValue = 0;
            int numValues = 0;
            uint idx = 0;
            for (auto possible : *cell_->getPossibleValues()) {
                // If we already have one value and we found another one,
                // then there's nothing to do
                if (possible == 1 && testValue > 0) {
                    return;
                } else if (possible == 1) {
                    testValue = idx;
                    numValues++;
                }
                idx++;
            }

            cell_->setSoleValue(testValue);
            broadcast("rm", testValue);
        }

        // if we got here, then we only have one value, so
        // send out a broadcast message to remove that value as a possibility
    } else if (command == "set") {
        // reset all other values, ensuring that only this value is set,
        // then send g, h and, v messages to reset this value in all other
        // relevant cells

        for (uint idx = 1; idx < (*cell_->getPossibleValues()).size(); idx++) {
            if (idx == value) {
                (*cell_->getPossibleValues())[idx] = 1;
            } else {
                (*cell_->getPossibleValues())[idx] = 0;
            }
        }
        broadcast("rm", value);
    }
}

void CellHub::broadcast(const string msg, const uint value) {
    for (io_ptr ioPort : ioPorts_)
    {
        ioPort->sendToExt(IoMessage(msg, value, ioPort->getDirection()));
    }
}
