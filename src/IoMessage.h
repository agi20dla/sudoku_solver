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

    std::string message_;
    uint value_;
    std::string direction_;

public:
    IoMessage();
    IoMessage(std::string message);

    IoMessage(std::string message, uint value);

    IoMessage(std::string message, uint value, std::string direction);

    string& getMessage();
    string& getDirection();

//    void setMsgUuid(boost::uuids::uuid uuid);
    boost::uuids::uuid getMsgUuid();

    void setRcvPortUuid(boost::uuids::uuid uuid);
    boost::uuids::uuid getRcvPortUuid();

    IoMessage& operator=(const IoMessage& other);

    uint getValue();
};



#endif //SUDOKU_SOLVER_MESSAGE_H
