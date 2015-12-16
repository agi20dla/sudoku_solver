//
// Created by jason on 5/22/15.
//

#include <string>
//#include "gmock/gmock.h"
#include "IoPort.h"
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

bool CellHub::run() {
    std::shared_ptr<IoMessage> ioMessage = tryPop();

    while (ioMessage != nullptr) {
        // send message to ports
        for (io_ptr ioPort : ioPorts_) {
            if (ioPort->getDirection() == ioMessage->getDirection()) {
                if (ioPort->sendToExt(ioMessage)) {
                    ++messagesSent_;
                }
            }
        }
        // save the command and the value
        string command = ioMessage->getCommand();
        uint value = ioMessage->getValue();
        // process command and value
        if (!processCommand(command, value)) {
            return false;
        }
        ioMessage = tryPop();
    }

    return true;
}


bool CellHub::processCommand(const string &command, const uint value) {
    if (command == "rm") {
        // if value being removed is the sole value, emit an exception
        if (cell_->getSoleValue() == value) {
            return false;
        }

        // remove this value as a possible solution
        (*cell_->getPossibleValues())[value] = 0;

        // if removing a value leaves only one possible value
        // then set this cell's sole value and
        // broadcast a rm message to other cells for that value
        if (cell_->getSoleValue() == 0) {
            uint testValue = 0;
            int numValues = 0;
            uint idx = 0;
            for (auto possible : *cell_->getPossibleValues()) {
                // If we already have one value and we found another one,
                // then there's nothing to do
                if (possible == 1 && testValue > 0) {
                    return true;
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
                cell_->setSoleValue(idx);
            } else {
                (*cell_->getPossibleValues())[idx] = 0;
            }
        }
        broadcast("rm", value);
    }

    return true;
}

void CellHub::broadcast(const string &command, const uint value) {
    for (io_ptr ioPort : ioPorts_)
    {
        ioPort->sendToExt(
                make_shared<IoMessage>(std::string(command), value, std::string(ioPort->getDirection()), uuid_));
    }
}
