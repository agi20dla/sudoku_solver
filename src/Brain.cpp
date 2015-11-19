//
// Created by jason on 6/5/15.
//

#include <string>
#include<vector>
#include <iostream>
#include "Brain.h"
#include "Exceptions.h"

class IoPort;

using namespace std;
using namespace boost;

//Brain::Brain()
//{}


/**
 * initialize a sudoku brain
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
void Brain::initialize() {

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

cell_ptr Brain::getCell(const uint row, const uint col) {
    cell_ptr c = cellMap_.at(row * 9 + col);
    return c;
}

void Brain::pushGlobal(cell_ptr global) {
    cell_ptr g = global;
    globalMap_.push_back(g);
}

cell_ptr Brain::getGlobal(const uint row, const uint col) {
    cell_ptr g = globalMap_.at(row * 6 + col);
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
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            auto cell = getCell(row, col);
            if (cell) {
                mgtPortMap_.push_back(cell->getMgtConnection("b"));
            }
        }
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

/**
 * Not all of these are used
 */
void Brain::createGlobalMap(){
    for (uint row = 0; row < 6; row++) {
        for (uint col = 0; col < 6; col++) {
            pushGlobal(make_shared<Cell>());
        }
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
            cell_ptr global = getGlobal(gRow, gCol);
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
    cout << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            if (auto cell = getCell(row, col)) {
                ulong connections = cell->numConnections();
                cout << connections << " ";
            }
        }
        cout << endl;
    }
}

void Brain::printValues() {
    cout << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            cout << "r:" << row << ",c:" << col << " - ";
            if (auto cell = getCell(row, col)) {
                vector<uint> values = cell->getValues();
                bool skip = true;
                for (uint v : values) {
                    if (skip) {
                        skip = false;
                        continue;
                    }
                    cout << v << " ";
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
    cout << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            if (auto cell = getCell(row, col)) {
                ulong msgsRcvd = cell->numMessagesRcvd();
                cout << msgsRcvd << " ";
            }
        }
        cout << endl;
    }
}


void Brain::run()
{
    // run the globals
    for (uint row = 0; row < 6; row++) {
        for (uint col = 0; col < 6; col++) {
            cell_ptr global = getGlobal(row, col);
            if (global.get() != nullptr) {
                getGlobal(row, col)->run();
            }
        }
    }

    // run the cells

    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            if (auto cell = getCell(row, col)) {
                cell->run();
            }
        }
    }
}


void Brain::setValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage("set", value, "b");
    io_ptr mgtPort = getMgtPort(row, col);
    mgtPort->fwdToQueue(ioMessage);
}

void Brain::removeValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage("rm", value, "m");
    getMgtPort(row, col)->fwdToQueue(ioMessage);
}

vector<uint> Brain::getValues(const uint row, const uint col)
{
    vector<uint> values;
    auto cell = getCell(row, col);
    if (cell) {
        values = cell->getValues();
    }
    return values;
}

