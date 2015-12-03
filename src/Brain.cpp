//
// Created by jason on 6/5/15.
//

#include <string>
#include<vector>
#include <iostream>
#include "Brain.h"
#include "GlobalCell.h"
#include "Exceptions.h"

class IoPort;

using namespace std;
using namespace boost;

//Brain::Brain()
//{}


/**
 * Reset a sudoku brain to the following map.
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

    puzzleCells_.clear();
    globalCells_.clear();
    brainPorts_.clear();

    createPuzzleCells();
    connectPuzzleRows();
    connectPuzzleCols();

    createGlobalCells();
    connectGlobals();

    connectBrainToPuzzleCells();
}


void Brain::pushCell(puzzle_cell_ptr cell) {
    puzzle_cell_ptr c = cell;
    puzzleCells_.push_back(c);
}

puzzle_cell_ptr Brain::getCell(const uint index) {
    puzzle_cell_ptr c = puzzleCells_.at(index);
    return c;
}

puzzle_cell_ptr Brain::getCell(const uint row, const uint col) {
    puzzle_cell_ptr c = puzzleCells_.at(row * 9 + col);
    return c;
}

void Brain::pushGlobal(global_cell_ptr global) {
    global_cell_ptr g = global;
    globalCells_.push_back(g);
}

global_cell_ptr Brain::getGlobal(const uint index) {
    global_cell_ptr g = globalCells_.at(index);
    return g;
}

global_cell_ptr Brain::getGlobal(const uint row, const uint col) {
    global_cell_ptr g = globalCells_.at(row * 6 + col);
    return g;
}

io_ptr Brain::getMgtPort(const uint row, const uint col) {
    io_ptr p = brainPorts_[row * 9 + col];
    return p;
}



/**
 * Just create the cell map by adding new cells to the vector of vectors
 */
void Brain::createPuzzleCells()
{
    for (uint idx = 0; idx < 81; idx++) {
        pushCell(make_shared<PuzzleCell>());
    }
}


void Brain::connectBrainToPuzzleCells() {
    for (auto c : puzzleCells_) {
        brainPorts_.push_back(c->connect("b"));
    }
}


void Brain::connectPuzzleRows()
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

void Brain::connectPuzzleCols() {
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


void Brain::createGlobalCells() {
    for (uint idx = 0; idx < 36; idx++) {
        pushGlobal(make_shared<GlobalCell>());
    }
}

/**
 * Connects all the Global Cells to their intended
 * Puzzle Cells
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
            global_cell_ptr global = getGlobal(gRow, gCol);
            // top left
            global->connect(getCell(row, col), "g");

            // top right
            global->connect(getCell(row, col+1), "g");

            // bottom left
            global->connect(getCell(row+1, col), "g");

            // bottom right
            global->connect(getCell(row+1, col+1), "g");
            gCol++;
        }
        gRow++;
    }
}


void Brain::printConnections() {
    // cell map connections
    cout << endl << "PuzzleCell connections" << endl;
    uint idx = 0;
    for (auto c : puzzleCells_) {
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
    cout << endl << "PuzzleCell values" << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            cout << "r:" << row << ",c:" << col << " - ";
            if (auto cell = getCell(row, col)) {
                vector<uint> *values = cell->getPossibleValues();
                bool skip = true;

                for (auto v : *values) {
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

void Brain::printNumMsgsRcvd()
{
    // cell map messages received count
    cout << endl << "Messages Received" << endl;
    uint idx = 0;
    for (auto c : puzzleCells_) {
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
 * TODO: Change test for number of messages to just be a bool on whether any cell has messages and
 * short circuit the check
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
        for (auto g : globalCells_) {
            g->run();
        }

        // run the cells
        for (auto c : puzzleCells_) {
            c->run();
        }

        if (debug) {
            cout << endl;
            cout << "Run number: " << numRuns << endl;
            printValues();
            cout << endl;
            printNumMsgsRcvd();
            printSolution();
        }

        numMsgsRemaining = 0;
        for (auto c : puzzleCells_) {
            numMsgsRemaining += c->numMessagesOnQueue();
        }
        for (auto g : globalCells_) {
            numMsgsRemaining += g->numMessagesOnQueue();
        }
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
    IoMessage ioMessage(string("set"), value, "b");
    io_ptr mgtPort = getMgtPort(row, col);
    mgtPort->fwdToQueue(ioMessage);
}

void Brain::removeValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage(string("rm"), value, "b");
    getMgtPort(row, col)->fwdToQueue(ioMessage);
}

vector<uint> *Brain::getValues(const uint row, const uint col)
{
    vector<uint> *values;
    auto cell = getCell(row, col);
    if (cell) {
        values = cell->getPossibleValues();
    }
    return values;
}

void Brain::printSolution() {
    cout << endl << "Solution" << endl;
    vector<std::string> solution;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {

            if (auto cell = getCell(row, col)) {
                auto values = cell->getPossibleValues();
                uint value = 0;
                uint numValues = 0;
                for (uint idx = 1; idx < 10; idx++) {
                    if ((*values).at(idx) == 1) {
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

std::vector<uint> Brain::getSolution() {
    vector<uint> solution;
    for (puzzle_cell_ptr cell : puzzleCells_)
    {
         solution.push_back(cell->getSoleValue());
    }
    return solution;
}
