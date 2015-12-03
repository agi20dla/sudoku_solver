//
// Created by jason on 11/23/15.
//

#include "Cell.h"
#include "CellHub.h"

using namespace std;

Cell::Cell()
        : rcvdMsgUUIDs_(make_shared<boost::unordered_map<boost::uuids::uuid, uint>>())
{
}

Cell::Cell(const Cell &other)
        : rcvdMsgUUIDs_(other.rcvdMsgUUIDs_)
{
}


/**
 * create a new port and send it to the other cell for a connection
 * The other cell will also create a new port, connect the ports, and
 * return it's port so we can connect to it.
 */
void Cell::connect(shared_ptr<Cell> otherCell, const string &direction) {
    io_ptr port = getMsgConnection(direction);

    if (otherCell) {
        io_ptr otherPort = otherCell->connect(port, direction);
        port->connect(otherPort);
    }
}


/**
 * create a management port connected to the messaging queue
 * send that port back so the caller can connect to us
 */
io_ptr Cell::connect(const std::string &direction) {
    io_ptr port = getMsgConnection(direction);
    return port;
}

io_ptr Cell::connect(io_ptr otherPort, const string &direction)
{
    io_ptr port = getMsgConnection(direction);
    port->connect(otherPort);
    return port;
}

Cell &Cell::operator=(const Cell &other) {
    if (this == &other) {
        return *this;
    }
    return *this;
}
