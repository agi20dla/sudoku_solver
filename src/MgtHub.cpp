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
        vector<string> tokens = split(ioMessage.getMessage(), ':');
        string cmd = tokens[0];
        uint value = atoi(tokens[1].c_str());
        if (cmd == "rm") {
            // reset the value
            (*possible_values_)[value] = 0;
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
            sendMsgToCell("rm:" + to_string(value));
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

std::vector<std::string>& MgtHub::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> MgtHub::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void MgtHub::addPossibleValues(std::vector<uint>* possible_values) {
    possible_values_ = possible_values;
}

/**
 * send out a message to the cell to do stuff
 */
void MgtHub::sendMsgToCell(string msg) {
    IoMessage globalMsg(msg, "g");
    IoMessage hMsg(msg, "h");
    IoMessage vMsg(msg, "v");

    for (shared_ptr<IoPort> ioPort : ioPorts_) {
        if (ioPort->getDirection() == "m") {
            ioPort->sendToExt(globalMsg);
            ioPort->sendToExt(hMsg);
            ioPort->sendToExt(vMsg);
        }
//        if (rcvPortUuid != ioPort->getUuid()
//            && ioPort->getDirection() == ioMessage.getDirection()
//            && ioPort->sendToExt(ioMessage))
//        {
//            ++Hub::messagesSent_;
//        }
    }
}
