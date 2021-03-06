//
// Created by jason on 6/17/15.
//

#include "Hub.h"
#include "Random.h"

using namespace std;

Hub::Hub()
        : messageQueue_(ConcurrentQueue<io_msg_ptr>()), ioPorts_(vector<io_ptr>()),
          uuid_(Random::getInstance().getNewUUID()),
          messagesSent_(0), messagesRcvd_(0)
{ }

Hub::Hub(const Hub &other)
        : messageQueue_(other.messageQueue_), ioPorts_(other.ioPorts_),
          uuid_(Random::getInstance().getNewUUID()),
          messagesSent_(other.messagesSent_), messagesRcvd_(other.messagesRcvd_)
{ }

Hub::~Hub()
{
    ioPorts_.clear();
    messageQueue_.clear();
}

bool Hub::run() {
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
        ioMessage = tryPop();
    }

    return true;
}

void Hub::addIoPort(io_ptr ioPort) {
    ioPorts_.push_back(ioPort);
}

bool Hub::hasMessages() {
    return messageQueue_.size() > 0;
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

std::shared_ptr<IoMessage> Hub::tryPop() {
    return messageQueue_.try_pop();
}

void Hub::push(std::shared_ptr<IoMessage> ioMessage) {
    messageQueue_.push(ioMessage);

}

void Hub::clear() {
    messageQueue_.clear();
}

const boost::uuids::uuid Hub::getUUID() {
    return uuid_;
}
