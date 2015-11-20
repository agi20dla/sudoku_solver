//
// Created by jason on 5/16/15.
//

#include <boost/uuid/uuid.hpp>
#include "IoMessage.h"

IoMessage::IoMessage()
        : message_(""), value_(0), direction_("") {
    { }
}

IoMessage::IoMessage(const string command, const uint value, const string direction)
        : message_(command), value_(value), direction_(direction) {
    boost::uuids::uuid uuid = uuidGenerator();
    uuid_ = uuid;
}

IoMessage::IoMessage(const string message, const uint value)
        : message_(message), value_(value), direction_("g")
{
    boost::uuids::uuid uuid = uuidGenerator();
    uuid_ = uuid;
}

IoMessage::IoMessage(const string message)
        : message_(message), value_(0)
        , direction_("g")
{
    boost::uuids::uuid uuid = uuidGenerator();
    uuid_ = uuid;
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

//void IoMessage::setMsgUuid(boost::uuids::uuid uuid) {
//    uuid_ = uuid;
//}

const boost::uuids::uuid IoMessage::getMsgUuid() {
    return uuid_;
}

//IoMessage &IoMessage::operator=(const IoMessage &other) {
//    if (this == &other) {
//        return *this;
//    }
//    this->message_ = other.message_;
//    this->value_ = other.value_;
//    this->uuid_ = other.uuid_;
//    this->direction_ = other.direction_;
//    this->rcvPortUuid_ = other.rcvPortUuid_;
//
//    return *this;
//}

void IoMessage::setRcvPortUuid(boost::uuids::uuid uuid) {
    rcvPortUuid_ = uuid;
}

boost::uuids::uuid IoMessage::getRcvPortUuid() {
    return rcvPortUuid_;
}
