//
// Created by jason on 5/16/15.
//

#include <boost/uuid/uuid.hpp>
#include <iostream>
#include "IoMessage.h"

IoMessage::IoMessage()
        : command_(""), value_(0), direction_("") { }

IoMessage::IoMessage(const IoMessage &other)
        : command_(other.command_), value_(other.value_),
          direction_(other.direction_) { }

IoMessage::IoMessage(const string &command, const uint value, const string &direction, const boost::uuids::uuid hubUUID)
        : command_(command), value_(value), direction_(direction)
{
    hubUuids_.insert(hubUUID);
}

IoMessage::~IoMessage()
{
    hubUuids_.clear();
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

bool IoMessage::getHubUUID(const boost::uuids::uuid uuid) {
    bool found = false;
    std::unordered_set<boost::uuids::uuid, boost::hash<boost::uuids::uuid>>::iterator it = hubUuids_.find(uuid);
    if (it != hubUuids_.end()) {
        found = true;
    }
    return found;
}

void IoMessage::addHubUUID(const boost::uuids::uuid &&uuid) {
    hubUuids_.insert(uuid);
}

IoMessage &IoMessage::operator=(const IoMessage &other) {
    if (this != &other) {
        command_ = other.command_;
        value_ = other.value_;
        direction_ = other.direction_;
        hubUuids_ = other.hubUuids_;
    }

    return *this;
}

IoMessage &IoMessage::operator=(IoMessage &&other) {
    assert(this != &other);
    if (this != &other) {
        command_ = other.command_;
        value_ = other.value_;
        direction_ = other.direction_;
        hubUuids_ = other.hubUuids_;
    }

    return *this;
}
