//
// Created by jason on 6/5/15.
//

#include <string>
#include<vector>
#include <iostream>
#include "Brain.h"
#include "BasicCell.h"
#include "Exceptions.h"

class IoPort;

using namespace std;
using namespace boost;

//Brain::Brain()
//{}


/**
 * reset a sudoku brain
 *
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v   v   v   v   v   v   v   v   v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v   v   v   v   v   v   v   v   v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 */
void Brain::reset() {

    cellMap_.clear();
    globalMap_.clear();
    mgtPortMap_.clear();

    createCellMap();
    connectRowCells();
    connectColumnCells();

    createGlobalMap();
    connectGlobals();

    connectBrainToCells();
}


void Brain::pushCell(cell_ptr cell) {
    cell_ptr c = cell;
    cellMap_.push_back(c);
}

cell_ptr Brain::getCell(const uint index) {
    cell_ptr c = cellMap_.at(index);
    return c;
}

cell_ptr Brain::getCell(const uint row, const uint col) {
    cell_ptr c = cellMap_.at(row * 9 + col);
    return c;
}

void Brain::pushGlobal(basic_cell_ptr global) {
    basic_cell_ptr g = global;
    globalMap_.push_back(g);
}

basic_cell_ptr Brain::getGlobal(const uint index) {
    basic_cell_ptr g = globalMap_.at(index);
    return g;
}

basic_cell_ptr Brain::getGlobal(const uint row, const uint col) {
    basic_cell_ptr g = globalMap_.at(row * 6 + col);
    return g;
}

io_ptr Brain::getMgtPort(const uint row, const uint col) {
    io_ptr p = mgtPortMap_[row * 9 + col];
    return p;
}



/**
 * Just create the cell map by adding new cells to the vector of vectors
 */
void Brain::createCellMap()
{
    for (uint idx = 0; idx < 81; idx++) {
        pushCell(make_shared<Cell>());
    }
}


/**
 * Create the map of IoPorts that will be used for sending management messages
 */
void Brain::connectBrainToCells() {
    for (auto c : cellMap_) {
        mgtPortMap_.push_back(c->getMgtConnection("b"));
    }
}


void Brain::connectRowCells()
{
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 8; col++) {
            auto cell1 = getCell(row, col);
            auto cell2 = getCell(row, col + 1);
            if (cell1 && cell2) {
                cell1->connect(cell2, "h");
            }
        }
    }
}

void Brain::connectColumnCells() {
    for (uint col = 0; col < 9; col++) {
        for (uint row = 0; row < 8; row++) {
            auto cell1 = getCell(row, col);
            auto cell2 = getCell(row + 1, col);
            if (cell1 && cell2) {
                cell1->connect(cell2, "v");
            }
        }
    }
}


void Brain::createGlobalMap(){
    for (uint idx = 0; idx < 36; idx++) {
        pushGlobal(make_shared<BasicCell>());
    }
}

/**
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v   v   v   v   v   v   v   v   v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v   v   v   v   v   v   v   v   v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 * v G v G v   v G v G v   v G v G v
 * C h C h C h C h C h C h C h C h C
 */
void Brain::connectGlobals() {
    uint gRow = 0;
    for (uint row = 0; row < 9; row++) {
        if ((row+1) % 3 == 0) {
            continue;
        }
        uint gCol = 0;
        for (uint col = 0; col < 9; col++) {
            if ((col +1) % 3 == 0) {
                continue;
            }
            basic_cell_ptr global = getGlobal(gRow, gCol);
            // top left
            global->connect(getCell(row, col));

            // top right
            global->connect(getCell(row, col+1));

            // bottom left
            global->connect(getCell(row+1, col));

            // bottom right
            global->connect(getCell(row+1, col+1));
            gCol++;
        }
        gRow++;
    }
}


void Brain::printConnections() {
    // cell map connections
    cout << endl << "Cell connections" << endl;
    uint idx = 0;
    for (auto c : cellMap_) {
        ulong connections = c->numConnections();
        cout << connections << " ";

        idx++;
        if (idx % 3 == 0) {
            cout << " ";
        }
        if (idx % 9 == 0) {
            cout << endl;
        }
        if (idx % 27 == 0) {
            cout << endl;
        }
    }
}

void Brain::printValues() {
    cout << endl << "Cell values" << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            cout << "r:" << row << ",c:" << col << " - ";
            if (auto cell = getCell(row, col)) {
                vector<int_ptr> *values = cell->getValues();
                bool skip = true;

                for (auto v : *values) {
                    if (skip) {
                        skip = false;
                        continue;
                    }
                    cout << *v << " ";
                }
            }
            cout << ":" << " ";
        }
        cout << endl;
    }
}

void Brain::printMessagesRcvd()
{
    // cell map messages received count
    cout << endl << "Messages Received" << endl;
    uint idx = 0;
    for (auto c : cellMap_) {
        ulong msgsRcvd = c->numMessagesRcvd();
        cout << msgsRcvd << " ";
        idx++;
        if (idx % 3 == 0) {
            cout << " ";
        }
        if (idx % 9 == 0) {
            cout << endl;
        }
        if (idx % 27 == 0) {
            cout << endl;
        }
    }
}

/**
 * Run until there's nothing else to do
 * TODO: The test for nothing else to do needs to be something other than whether
 * the number of messages is the same from one iteration to the next.  Perhaps try
 * the number of queued messages in each cell's hub.
 */
void Brain::run(bool debug)
{
    bool firstRun = true;
    ulong numMsgsRemaining = 0;
    ulong numRuns = 0;
    while (firstRun || numMsgsRemaining > 0) {
        firstRun = false;
        numRuns++;
        // run the globals
        for (auto g : globalMap_) {
            g->run();
        }

        // run the cells
        for (auto c : cellMap_) {
            c->run();
        }

        if (debug) {
            cout << endl;
            cout << "Run number: " << numRuns << endl;
            printValues();
            cout << endl;
            printSolution();
            printMessagesRcvd();
        }

        numMsgsRemaining = 0;
        for (auto c : cellMap_) {
            numMsgsRemaining += c->numMessages();
        }
        for (auto g : globalMap_) {
            numMsgsRemaining += g->numMessages();
        }
    }

    for (auto c : cellMap_) {
        c->getValues();
    }

    cout << endl << "Number of runs: " << numRuns << endl;
}


void Brain::setValue(const uint row, const uint col, const uint value)
{
    // value may be 0 during initial puzzle setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    msg_ptr ioMessage = make_shared<IoMessage>(string("set"), value, "b");
    io_ptr mgtPort = getMgtPort(row, col);
    mgtPort->fwdToQueue(ioMessage);
}

void Brain::removeValue(const uint row, const uint col, const uint value)
{
    msg_ptr ioMessage = make_shared<IoMessage>(string("rm"), value, "m");
    getMgtPort(row, col)->fwdToQueue(ioMessage);
}

vector<int_ptr> *Brain::getValues(const uint row, const uint col)
{
    vector<int_ptr> *values;
    auto cell = getCell(row, col);
    if (cell) {
        values = cell->getValues();
    }
    return values;
}

void Brain::printSolution() {
    cout << endl << "Solution" << endl;
    vector<string> solution;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {

            if (auto cell = getCell(row, col)) {
                auto values = cell->getValues();
                uint value = 0;
                uint numValues = 0;
                for (uint idx = 1; idx < 10; idx++) {
                    if (*((*values).at(idx)) == 1) {
                        numValues++;
                        value = idx;
                    }
                }
                if (numValues == 1) {
                    solution.push_back(to_string(value));
                } else {
                    solution.push_back("*");
                }
            }
        }
    }

    uint idx = 0;
    for (auto out : solution) {
        cout << out << " ";

        idx++;
        if (idx % 3 == 0) {
            cout << " ";
        }
        if (idx % 9 == 0) {
            cout << endl;
        }
        if (idx % 27 == 0) {
            cout << endl;
        }
    }
}

void Brain::initialize(const vector<uint> values) {
    uint row = 0;
    uint col = 0;
    for (auto value : values) {
        setValue(row, col, value);
        col++;
        if (col == 9) {
            col = 0;
            row++;
        }
    }
}
