//
// Created by jason on 5/16/15.
//

#ifndef SUDOKU_SOLVER_MESSAGE_H
#define SUDOKU_SOLVER_MESSAGE_H


#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_set>

using namespace std;

class IoMessage {

private:
    std::string command_;
    uint value_;
    std::string direction_;
    std::unordered_set<boost::uuids::uuid, boost::hash<boost::uuids::uuid>> hubUuids_;

public:
    IoMessage();

    IoMessage(const IoMessage &other);

    IoMessage(const std::string &command, const uint value, const std::string &direction,
              const boost::uuids::uuid hubUUID);

    virtual ~IoMessage();

    const string getCommand();

    const uint getValue();

    const string getDirection();

    bool getHubUUID(const boost::uuids::uuid uuid);

    void addHubUUID(const boost::uuids::uuid &&uuid);

    IoMessage &operator=(const IoMessage &other);

    IoMessage &operator=(IoMessage &&other);
};



#endif //SUDOKU_SOLVER_MESSAGE_H
