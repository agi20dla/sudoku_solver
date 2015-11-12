//
// Created by jason on 6/5/15.
//

#include <string>
#include<vector>

//#include <boost/range/irange.hpp>
//#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>
#include "Brain.h"

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


void Brain::pushCell(std::shared_ptr<Cell> cell) {
    cellMap_.push_back(cell);
}

std::shared_ptr<Cell> Brain::getCell(const uint row, const uint col) {
    return cellMap_[row*9+col];
}

void Brain::pushGlobal(std::shared_ptr<Cell> cell) {
    globalMap_.push_back(cell);
}

std::shared_ptr<Cell> Brain::getGlobal(const uint row, const uint col) {
    return globalMap_[row * 6 + col];
}

std::shared_ptr<IoPort> Brain::getMgtPort(const uint row, const uint col) {
    return mgtPortMap_[row*9+col];
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
            mgtPortMap_.push_back(getCell(row, col)->getMgtConnection("b"));
        }
    }
}


void Brain::connectRowCells()
{
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 8; col++) {
            std::shared_ptr<Cell> cell1 = getCell(row, col);
            std::shared_ptr<Cell> cell2 = getCell(row, col+1);
            cell1->connect(cell2, "h");
        }
    }
}

void Brain::connectColumnCells() {
    for (uint col = 0; col < 9; col++) {
        for (uint row = 0; row < 8; row++) {
            std::shared_ptr<Cell> cell1 = getCell(row, col);
            std::shared_ptr<Cell> cell2 = getCell(row+1, col);
            cell1->connect(cell2, "v");
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
            std::shared_ptr<Cell> global = getGlobal(gRow, gCol);
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
            ulong connections = getCell(row, col)->numConnections();
            cout << connections << " ";
        }
        cout << endl;
    }
}

void Brain::printValues() {
    cout << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            cout << "r:" << row << ",c:" << col << " - ";
            vector<uint> values = getCell(row, col)->getValues();
            bool skip = true;
            for (uint v : values) {
                if (skip) {
                    skip = false;
                    continue;
                }
                cout << v << " ";
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
            ulong msgsRcvd = getCell(row, col)->numMessagesRcvd();
            cout << msgsRcvd << " ";
        }
        cout << endl;
    }
}


void Brain::run()
{
    // run the globals
    for (uint row = 0; row < 6; row++) {
        for (uint col = 0; col < 6; col++) {
            getGlobal(row, col)->run();
        }
    }

    // run the cells
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            getCell(row, col)->run();
        }
    }
}


void Brain::setValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage("set:" + to_string(value), "b");
    getMgtPort(row, col)->fwdToQueue(ioMessage);
}

void Brain::removeValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage("rm:" + to_string(value), "m");
    getMgtPort(row, col)->fwdToQueue(ioMessage);
}

vector<uint> Brain::getValues(const uint row, const uint col)
{
    return getCell(row, col)->getValues();
}

