//
// Created by jason on 5/22/15.
//

#include <string>
#include "gmock/gmock.h"
#include "../src/IoPort.h"
#include "CellHub.h"
#include "Hub.h"

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
                && ioPort->getDirection() == ioMessage.getDirection()
                && ioPort->sendToExt(ioMessage))
            {
                ++Hub::messagesSent_;
            }
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

