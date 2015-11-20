//
// Created by jason on 6/17/15.
//

#include "MgtHub.h"
#include "IoPort.h"
#include "Exceptions.h"

/**
 * Process "m" messages
 */
void MgtHub::run() {
    msg_ptr ioMessage;

    while ((ioMessage = tryPop())) {
        string cmd = ioMessage->getMessage();
        uint value = ioMessage->getValue();
        if (cmd == "rm") {
            // remove this value as a possible solution
            (*(*possible_values_)[value]) = 0;

            // if removing a value leaves only one possible value
            // then send a rm message to the cell for that value
            if (soleValue == 0) {
                uint testValue = 0;
                int numValues = 0;
                uint idx = 0;
                for (auto possible : (*possible_values_)) {
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
        } else if (cmd == "set") {
            // reset all other values, ensuring that only this value is set,
            // then send g, h and, v messages to reset this value in all other
            // relevant cells
            uint idx = 0;
            for (auto possible : (*possible_values_)) {
                if (idx == value) {
                    *possible = 1;
                } else {
                    *possible = 0;
                }
                idx++;
            }
//            for (uint idx = 1; idx <= 9; idx++) {
//                if (idx == value) {
//                    (*possible_values_)[idx] = 1;
//                } else {
//                    (*possible_values_)[idx] = 0;
//                }
//            }
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

void MgtHub::addPossibleValues(std::vector<uint *> *possible_values) {
    possible_values_ = possible_values;
}

/**
 * send out a message to the cell to do stuff
 */
void MgtHub::broadcast(const string msg, const uint value) {
    msg_ptr globalMsg = make_shared<IoMessage>(msg, value, "g");
    msg_ptr hMsg = make_shared<IoMessage>(msg, value, "h");
    msg_ptr vMsg = make_shared<IoMessage>(msg, value, "v");

    for (io_ptr ioPort : ioPorts_) {
        if (ioPort->getDirection() == "m") {
            ioPort->sendToExt(globalMsg);
            ioPort->sendToExt(hMsg);
            ioPort->sendToExt(vMsg);
            break;
        }
    }
}
