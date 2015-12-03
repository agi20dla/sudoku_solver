//
// Created by jason on 6/17/15.
//

#include "Hub.h"
#include "IoPort.h"

using namespace std;

Hub::Hub()
        : messageQueue_(ConcurrentQueue<IoMessage>()), ioPorts_(vector<io_ptr>()), messagesSent_(0), messagesRcvd_(0),
          numPorts_(0) { }

Hub::Hub(const Hub &other)
        : messageQueue_(other.messageQueue_), ioPorts_(other.ioPorts_), messageUUIDs(other.messageUUIDs),
          messagesSent_(other.messagesSent_), messagesRcvd_(other.messagesRcvd_), numPorts_(0) { }

void Hub::run() {
    IoMessage ioMessage;

    while (tryPop(ioMessage)) {
        // send message to ports
        boost::uuids::uuid rcvPortUuid = ioMessage.getRcvPortUuid();
        for (io_ptr ioPort : ioPorts_) {
            if (rcvPortUuid != ioPort->getUuid()
                && ioPort->getDirection() == ioMessage.getDirection()) {
                if (ioPort->sendToExt(ioMessage)) {
                    ++messagesSent_;
                }
            }
        }
    }
}

void Hub::addIoPort(io_ptr ioPort) {
    ioPorts_.push_back(ioPort);
    numPorts_++;
}

ulong Hub::numMessagesOnQueue() {
    return messageQueue_.size();
}

ulong Hub::numMessagesSent() {
    return messagesSent_;
}

ulong Hub::numMessagesRcvd() {
    return messageQueue_.get_num_messages_rcvd();
}

ulong Hub::numPorts() {
    return numPorts_;
}

bool Hub::tryPop(IoMessage &popped) {
    return messageQueue_.try_pop(popped);
}

void Hub::push(IoMessage ioMessage) {
    messageQueue_.push(ioMessage);

}
