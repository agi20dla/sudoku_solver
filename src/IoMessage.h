//
// Created by jason on 5/16/15.
//

#ifndef SUDOKU_SOLVER_MESSAGE_H
#define SUDOKU_SOLVER_MESSAGE_H


#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

using namespace std;

class IoMessage {

private:
    std::string command_;
    uint value_;
    std::string direction_;
    boost::uuids::uuid rcvPortUuid_;
    boost::uuids::uuid uuid_;
    boost::uuids::random_generator uuidGenerator_;

public:
    IoMessage();

    IoMessage(const IoMessage& other);

    IoMessage(const std::string command, const uint value, const std::string direction);

    const string getCommand();

    const uint getValue();

    const string getDirection();

    const boost::uuids::uuid getUuid();

    // Stores the uuid of the port that forwarded this message, so the hub can avoid sending a message right back to that port
    void setForwardingPortUUID(boost::uuids::uuid uuid);

    boost::uuids::uuid getForwardingPortUUID();

    IoMessage& operator=(const IoMessage& other);
};



#endif //SUDOKU_SOLVER_MESSAGE_H
