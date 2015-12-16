//
// Created by jason on 6/17/15.
//

#include "Hub.h"

using namespace std;

Hub::Hub()
        : messageQueue_(ConcurrentQueue()), ioPorts_(vector<io_ptr>()), messagesSent_(0), messagesRcvd_(0),
          numPorts_(0) { }

Hub::Hub(const Hub &other)
        : messageQueue_(other.messageQueue_), ioPorts_(other.ioPorts_), messageUUIDs(other.messageUUIDs),
          messagesSent_(other.messagesSent_), messagesRcvd_(other.messagesRcvd_), numPorts_(0) { }

bool Hub::run() {
    std::shared_ptr<IoMessage> ioMessage = tryPop();
    while (ioMessage) {
        // send message to ports
        boost::uuids::uuid fwdPortUuid = ioMessage->getForwardingPortUUID();
        for (io_ptr ioPort : ioPorts_) {
            // but not back to the port that sent us this message
            if (fwdPortUuid != ioPort->getUuid()
                && ioPort->getDirection() == ioMessage->getDirection()) {
                if (ioPort->sendToExt(ioMessage)) {
                    ++messagesSent_;
                }
            }
        }
        ioMessage = tryPop();
    }

    return true;
}

void Hub::addIoPort(io_ptr ioPort) {
    ioPorts_.push_back(ioPort);
    numPorts_++;
}

ulong Hub::getNumMsgsOnQueue() {
    return messageQueue_.size();
}

ulong Hub::getNumMsgsSent() {
    return messagesSent_;
}

ulong Hub::getNumMsgsRcvd() {
    return messageQueue_.get_num_messages_rcvd();
}

ulong Hub::getNumPorts() {
    return numPorts_;
}

std::shared_ptr<IoMessage> Hub::tryPop() {
    return messageQueue_.try_pop();
}

void Hub::push(std::shared_ptr<IoMessage> ioMessage) {
    messageQueue_.push(ioMessage);

}

void Hub::clear() {
    messageQueue_.clear();
}
