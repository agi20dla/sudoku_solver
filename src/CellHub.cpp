//
// Created by jason on 5/22/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/IoPort.h"
#include "CellHub.h"

using namespace std;

/**
 * Iterate through all the messages on the message queue and send to the IO ports
 */
void CellHub::run() {
    IoMessage ioMessage;

    while(Hub::tryPop(ioMessage)) {
        boost::uuids::uuid rcvPortUuid = ioMessage.getRcvPortUuid();
        for (shared_ptr<IoPort> ioPort : ioPorts_) {
            if (rcvPortUuid != ioPort->getUuid()
                && ioPort->getDirection() == ioMessage.getDirection())
            {
                if (ioPort->sendToExt(ioMessage)) {
                    ++Hub::messagesSent_;
                }
            }
        }
        sendMsgToMgt(ioMessage);
    }
}

/**
 * send out a message to the mgt hub to do stuff with the values
 */
void CellHub::sendMsgToMgt(IoMessage msg) {
    for (shared_ptr<IoPort> ioPort : ioPorts_) {
        if (ioPort->getDirection() == "m") {
            ioPort->sendToExt(msg);
            break;
        }
    }
}

CellHub::CellHub() : Hub()
{
}

CellHub::CellHub(const CellHub &other)
: Hub(other)
{
}



CellHub &CellHub::operator=(const CellHub & other) {
    Hub::operator=(other);
    return *this;
}

