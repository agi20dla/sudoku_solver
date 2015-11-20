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
    boost::uuids::uuid uuid_;
    boost::uuids::uuid rcvPortUuid_;
    boost::uuids::random_generator uuidGenerator;

    const std::string message_;
    const uint value_;
    const std::string direction_;

public:
    IoMessage();

    IoMessage(const std::string message);

    IoMessage(const std::string message, const uint value);

    IoMessage(const std::string message, const uint value, const std::string direction);

    const string getMessage();

    const string getDirection();

//    void setMsgUuid(boost::uuids::uuid uuid);
    const boost::uuids::uuid getMsgUuid();

    void setRcvPortUuid(boost::uuids::uuid uuid);
    boost::uuids::uuid getRcvPortUuid();

    IoMessage& operator=(const IoMessage& other);

    const uint getValue();
};



#endif //SUDOKU_SOLVER_MESSAGE_H
