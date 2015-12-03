//
// Created by jason on 5/16/15.
//

#include <boost/uuid/uuid.hpp>
#include "IoMessage.h"

IoMessage::IoMessage()
        : message_(""), value_(0), direction_("")
{
     uuid_ = uuidGenerator_();
}

IoMessage::IoMessage(const IoMessage& other)
        : message_(other.message_)
        , value_(other.value_)
        , direction_(other.direction_)
        , rcvPortUuid_(other.rcvPortUuid_)
        , uuid_(other.uuid_)
        , uuidGenerator_(other.uuidGenerator_)
{
}

IoMessage::IoMessage(const string command, const uint value, const string direction)
        : message_(command), value_(value), direction_(direction)
{
    uuid_ = uuidGenerator_();
}


const string IoMessage::getMessage() {
    return message_;
}

const string IoMessage::getDirection() {
    return direction_;
}

const uint IoMessage::getValue() {
    return value_;
}

const boost::uuids::uuid IoMessage::getMsgUuid() {
    return uuid_;
}

IoMessage &IoMessage::operator=(const IoMessage& other) {
    if (this == &other) {
        return *this;
    }
    message_ = other.message_;
    value_ = other.value_;
    direction_ = other.direction_;
    rcvPortUuid_ = other.rcvPortUuid_;
    uuid_ = other.uuid_;
    return *this;
}

void IoMessage::setRcvPortUuid(boost::uuids::uuid uuid) {
    rcvPortUuid_ = uuid;
}

boost::uuids::uuid IoMessage::getRcvPortUuid() {
    return rcvPortUuid_;
}
