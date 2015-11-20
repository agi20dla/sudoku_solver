//
// Created by jason on 6/17/15.
//

#include "MgtHub.h"
#include "IoPort.h"

/**
 * Process "m" messages
 */
void MgtHub::run() {
    IoMessage ioMessage;

    while(tryPop(ioMessage)) {
        string cmd = ioMessage.getMessage();
        uint value = ioMessage.getValue();
        if (cmd == "rm") {
            // reset the value
            (*possible_values_)[value] = 0;
            // if removing a value leaves only one possible value
            // then send a rm message to the cell for that value
            if (soleValue == 0) {
                uint testValue = 0;
                int numValues = 0;
                for (uint idx = 1; idx <= 9; idx++) {
                    // If we already have one value and we found another one,
                    // then there's nothing to do
                    if ((*possible_values_)[idx] == 1 && testValue > 0) {
                        return;
                    } else if ((*possible_values_)[idx] == 1) {
                        testValue = idx;
                        numValues++;
                    }
                }

                soleValue = testValue;
                broadcast("rm", soleValue);
            }


            // if we got here, then we only have one value, so
            // send out a broadcast message to remove that value as a possibility
        } else if (cmd == "set") {
            // reset all other values, ensuring that only this value is set,
            // then send g, h and, v messages to reset this value in all other
            // relevant cells
            for (uint idx = 1; idx <= 9; idx++) {
                if (idx == value) {
                    (*possible_values_)[idx] = 1;
                } else {
                    (*possible_values_)[idx] = 0;
                }
            }
            broadcast("rm", value);
        }
    }
}


MgtHub::MgtHub()
{
}


MgtHub::MgtHub(const MgtHub &other)
        : Hub(other)
{
}


MgtHub &MgtHub::operator=(const MgtHub & other) {
    Hub::operator=(other);
    return *this;
}

void MgtHub::addPossibleValues(std::vector<uint>* possible_values) {
    possible_values_ = possible_values;
}

/**
 * send out a message to the cell to do stuff
 */
void MgtHub::broadcast(string msg, uint value) {
    IoMessage globalMsg(msg, value, "g");
    IoMessage hMsg(msg, value, "h");
    IoMessage vMsg(msg, value, "v");

    for (io_ptr ioPort : ioPorts_) {
        if (ioPort->getDirection() == "m") {
            ioPort->sendToExt(globalMsg);
            ioPort->sendToExt(hMsg);
            ioPort->sendToExt(vMsg);
            break;
        }
    }
}
