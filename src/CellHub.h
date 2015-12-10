//
// Created by jason on 5/22/15.
//

#ifndef SUDOKU_SOLVER_CELL_HUB_H
#define SUDOKU_SOLVER_CELL_HUB_H

#include <memory>

#include <boost/uuid/uuid.hpp>
#include <boost/unordered_map.hpp>

#include "ConcurrentQueue.h"
#include "IoMessage.h"
#include "IoPort.h"
#include "Hub.h"
#include "Cell.h"

class CellHub : public Hub {
private:

    PuzzleCell *cell_;

public:
    CellHub();
    CellHub(const CellHub & other);

    // Iterates through all the messages on the message queue, calls processCommand
    // and forwards the message along the appropriate ports
    bool run() override;

    // Sets this hub's owning cell
    void setCell(PuzzleCell *cell);

    // Processes the given command, e.g. "rm" or "set" for the given value
    bool processCommand(const string& command, const uint value);

    // Sends out a message with the given command and value to all
    // ports connected to this hub
    void broadcast(const string command, const uint value);
};


#endif //SUDOKU_SOLVER_HUB_H
