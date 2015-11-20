//
// Created by jason on 6/17/15.
//

#include "Hub.h"

using namespace std;

Hub::Hub()
        : messageQueue_(make_shared<ConcurrentQueue<msg_ptr>>())
        , messagesSent_(0)
        , messagesRcvd_(0)
        , numPorts_(0)
{}

Hub::Hub(const Hub &other)
        : messageQueue_(other.messageQueue_)
        , ioPorts_(other.ioPorts_)
        , msgsReceived_(other.msgsReceived_)
        , messagesSent_(other.messagesSent_)
        , messagesRcvd_(other.messagesRcvd_)
        , numPorts_(0)
{}


void Hub::addIoPort(io_ptr ioPort) {
    ioPorts_.push_back(ioPort);
    numPorts_++;
}

ulong Hub::numMessagesOnQueue()
{
    return messageQueue_->size();
}

ulong Hub::numMessagesSent() {
    return messagesSent_;
}

ulong Hub::numMessagesRcvd() {
    return messageQueue_->get_num_messages_rcvd();
}

ulong Hub::numPorts() {
    return numPorts_;
}

Hub &Hub::operator=(const Hub & other) {
    if (this == &other) {
        return *this;
    }

    this->ioPorts_ = other.ioPorts_;
    this->messageQueue_ = other.messageQueue_;
    this->msgsReceived_ = other.msgsReceived_;

    return *this;
}


msg_ptr Hub::tryPop() {
    return messageQueue_->try_pop();
}

void Hub::push(msg_ptr ioMessage) {
    messageQueue_->push(ioMessage);

}