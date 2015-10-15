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
    return globalMap_[row*8+col];
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


void Brain::createGlobalMap(){
    for (uint row = 0; row < 8; row++) {
        for (uint y = 0; y < 8; y++) {
            pushGlobal(make_shared<Cell>());
        }
    }
}

void Brain::connectGlobals() {
    for (uint row = 0; row < 9; row++) {
        if ((row+1) % 3 == 0) {
            continue;
        }
        for (uint col = 0; col < 9; col++) {
            if ((col +1) % 3 == 0) {
                continue;
            }
            std::shared_ptr<Cell> global = getGlobal(row, col);
            // top left
            global->connect(getCell(row, col));

            // top right
            global->connect(getCell(row, col+1));

            // bottom left
            global->connect(getCell(row+1, col));

            // bottom right
            global->connect(getCell(row+1, col+1));
        }
    }
}


//void Brain::printConnections() {
//    // cell map connections
//    cout << endl;
//    for (uint row = 0; row < 9; row++) {
//        for (uint col = 0; col < 9; col++) {
//            ulong connections = cellMap_[row][col].numConnections();
//            cout << connections << " ";
//        }
//        cout << endl;
//    }
//}

void Brain::printMessages()
{
    // cell map messages received count
    cout << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            ulong connections = getCell(row, col)->numMessagesRcvd();
            cout << connections << " ";
        }
        cout << endl;
    }
}


void Brain::run()
{
    for (uint i = 0; i < 3; i++) {
        for (uint row = 0; row < 9; row++) {
            for (uint col = 0; col < 9; col++) {
                getCell(row, col)->run();
            }
        }
        for (uint row = 0; row < 8; row++) {
            for (uint col = 0; col < 8; col++) {
                getGlobal(row, col)->run();
            }
        }
        printMessages();
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

std::shared_ptr<vector<uint>> Brain::getValues(const uint row, const uint col)
{
    return getCell(row, col)->getValues();
}

