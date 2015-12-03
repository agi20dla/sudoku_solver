//
// Created by jason on 5/16/15.
//

#include <boost/uuid/uuid.hpp>
#include "IoMessage.h"

IoMessage::IoMessage()
        : command_(""), value_(0), direction_("")
{
     uuid_ = uuidGenerator_();
}

IoMessage::IoMessage(const IoMessage& other)
        : command_(other.command_)
        , value_(other.value_)
        , direction_(other.direction_)
        , rcvPortUuid_(other.rcvPortUuid_)
        , uuid_(other.uuid_)
        , uuidGenerator_(other.uuidGenerator_)
{
}

IoMessage::IoMessage(const string command, const uint value, const string direction)
        : command_(command), value_(value), direction_(direction)
{
    uuid_ = uuidGenerator_();
}


const string IoMessage::getCommand() {
    return command_;
}

const string IoMessage::getDirection() {
    return direction_;
}

const uint IoMessage::getValue() {
    return value_;
}

const boost::uuids::uuid IoMessage::getUuid() {
    return uuid_;
}

IoMessage &IoMessage::operator=(const IoMessage& other) {
    if (this == &other) {
        return *this;
    }
    command_ = other.command_;
    value_ = other.value_;
    direction_ = other.direction_;
    rcvPortUuid_ = other.rcvPortUuid_;
    uuid_ = other.uuid_;
    return *this;
}

void IoMessage::setForwardingPortUUID(boost::uuids::uuid uuid) {
    rcvPortUuid_ = uuid;
}

boost::uuids::uuid IoMessage::getForwardingPortUUID() {
    return rcvPortUuid_;
}
