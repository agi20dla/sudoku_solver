//
// Created by jason on 5/16/15.
//

#include <boost/uuid/uuid.hpp>
#include "IoMessage.h"

IoMessage::IoMessage() { }

IoMessage::IoMessage(string message, string direction)
        : message_(message)
        , direction_(direction)
{
    boost::uuids::uuid uuid = uuidGenerator();
    uuid_ = uuid;
}

IoMessage::IoMessage(string message)
        : message_(message)
        , direction_("g")
{
    boost::uuids::uuid uuid = uuidGenerator();
    uuid_ = uuid;
}

string &IoMessage::getMessage() {
    return message_;
}

string &IoMessage::getDirection() {
    return direction_;
}

//void IoMessage::setMsgUuid(boost::uuids::uuid uuid) {
//    uuid_ = uuid;
//}

boost::uuids::uuid IoMessage::getMsgUuid() {
    return uuid_;
}

IoMessage &IoMessage::operator=(const IoMessage &other) {
    if (this == &other) {
        return *this;
    }
    this->message_ = other.message_;
    this->uuid_ = other.uuid_;
    this->direction_ = other.direction_;
    this->rcvPortUuid_ = other.rcvPortUuid_;

    return *this;
}

void IoMessage::setRcvPortUuid(boost::uuids::uuid uuid) {
    rcvPortUuid_ = uuid;
}

boost::uuids::uuid IoMessage::getRcvPortUuid() {
    return rcvPortUuid_;
}
