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

    firstRun_ = true;
}


void Brain::createPuzzleCells()
{
    for (uint idx = 0; idx < 81; idx++) {
        puzzle_cell_ptr c(make_shared<PuzzleCell>());
        puzzleCells_.push_back(c);
    }
}


void Brain::connectPuzzleRows()
{
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 8; col++) {
            auto cell1 = getPuzzleCell(row, col);
            auto cell2 = getPuzzleCell(row, col + 1);
            if (cell1 && cell2) {
                cell1->connect(cell2, "h");
            }
        }
    }
}


void Brain::connectPuzzleCols() {
    for (uint col = 0; col < 9; col++) {
        for (uint row = 0; row < 8; row++) {
            auto cell1 = getPuzzleCell(row, col);
            auto cell2 = getPuzzleCell(row + 1, col);
            if (cell1 && cell2) {
                cell1->connect(cell2, "v");
            }
        }
    }
}


void Brain::createGlobalCells() {
    for (uint idx = 0; idx < 36; idx++) {
        global_cell_ptr g(make_shared<GlobalCell>());
        globalCells_.push_back(g);
    }
}


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
            global_cell_ptr global = getGlobalCell(gRow, gCol);
            // top left
            global->connect(getPuzzleCell(row, col), "g");

            // top right
            global->connect(getPuzzleCell(row, col + 1), "g");

            // bottom left
            global->connect(getPuzzleCell(row + 1, col), "g");

            // bottom right
            global->connect(getPuzzleCell(row + 1, col + 1), "g");
            gCol++;
        }
        gRow++;
    }
}


void Brain::connectBrainToPuzzleCells() {
    for (auto c : puzzleCells_) {
        brainPorts_.push_back(c->connect("b"));
    }
}


puzzle_cell_ptr Brain::getPuzzleCell(const uint row, const uint col) {
    puzzle_cell_ptr c = puzzleCells_.at(row * 9 + col);
    return c;
}


global_cell_ptr Brain::getGlobalCell(const uint row, const uint col) {
    global_cell_ptr g = globalCells_.at(row * 6 + col);
    return g;
}


io_ptr Brain::getBrainPort(const uint row, const uint col) {
    io_ptr p = brainPorts_[row * 9 + col];
    return p;
}


void Brain::printValues() {
    cout << "PuzzleCell values" << endl;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            cout << "r:" << row << ",c:" << col << " - ";
            if (auto cell = getPuzzleCell(row, col)) {
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
    cout << "Messages Received" << endl;
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


int Brain::solve(bool debug)
{
    cout << "Solving Sudoku Puzzle:" << endl;
    printSolution();

    if (firstRun_) {
        try {
            // Run first time
            run(debug);
            firstRun_ = false;
        } catch (const attempt_to_remove_sole_value &e) {
            cerr << "Attempeted to solve bad puzzle" << endl;
            return 1;
        }
    }

    if (isPuzzleSolved()) {
        cout << "Puzzle solved!" << endl;
        printSolution();
        return 0;
    }

    // store puzzle state and possible solutions
    solutionPath solPath{getSolutionStruct(), getPossibleSolutions()};
    solutionPaths_.push_back(solPath);

    // brute force a solution
    while (!isPuzzleSolved() && solutionPaths_.size() > 0) {
        cout << "Exploring solution:" << endl;
        printSolution();

        solutionPath path = solutionPaths_.back();
        solutionPaths_.pop_back();
        vector<valStruct> possibles = path.possibles;
        vector<puzzleState> puzzleStates = path.states;

        for (valStruct ss : possibles) {
            setValue(ss.row, ss.col, ss.val);
            try {
                run(debug);
                if (isPuzzleSolved()) {
                    break;
                } else {
                    // finished running with no exception, but no solution
                    // so save this solution for the next iteration
                    solutionPath solPath{getSolutionStruct(), getPossibleSolutions()};
                    solutionPaths_.push_back(solPath);
                }
            } catch (const attempt_to_remove_sole_value &e) {
                // This exception happens when we've tried to run down a bad path
                // reset all the cells for the next possible value
                uint idx = 0;
                for (auto c : puzzleCells_) {
                    c->reset(puzzleStates.at(idx).soleValue, puzzleStates.at(idx).possibles);
                    idx++;
                }
            }
        }
    }

    if (isPuzzleSolved()) {
        cout << "Puzzle solved!" << endl;
        printSolution();
    }
    return 0;
}


long Brain::run(bool debug) {
    bool firstRun = true;
    bool msgsRemaining = true;

    while (firstRun || msgsRemaining) {
        firstRun = false;
        numRuns_++;
        // run the globals
        for (auto g : globalCells_) {
            g->run();
        }

        // run the cells
        for (auto c : puzzleCells_) {
            c->run();
        }

        if (numRuns_ % 5 == 0 || debug) {
            cout << endl;
            cout << "Run number: " << numRuns_ << endl;
            printSolution();
        }
        if (debug) {
            printValues();
            printNumMsgsRcvd();
            printSolution();
        }

        for (auto c : puzzleCells_) {
            if (c->numMessagesOnHub() > 0) {
                msgsRemaining = true;
                break;
            } else {
                msgsRemaining = false;
            }
        }
        if (!msgsRemaining) {
            for (auto g : globalCells_) {
                if (g->numMessagesOnHub() > 0) {
                    msgsRemaining = true;
                    break;
                }
            }
        }
    }

    return numRuns_;
}


void Brain::setValue(const uint row, const uint col, const uint value)
{
    // value may be 0 during initial puzzle setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    getPuzzleCell(row, col)->setSoleValue(value);
    IoMessage ioMessage(string("set"), value, "b");
    io_ptr mgtPort = getBrainPort(row, col);
    mgtPort->fwdToQueue(ioMessage);
}


void Brain::setValue(const uint row, const uint col, const uint value, const vector<uint> possibles) {
    // value may be 0 during initial puzzle setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    auto c = getPuzzleCell(row, col);
    c->setSoleValue(value);
    c->setPossibles(possibles);
}


void Brain::setValues(const vector<puzzleState> solution) {
    uint row = 0;
    uint col = 0;
    for (auto s : solution) {
        setValue(row, col, s.soleValue, s.possibles);
        col++;
        if (col == 9) {
            col = 0;
            row++;
        }
    }
}

void Brain::setValues(const vector<uint> values) {
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


void Brain::removeValue(const uint row, const uint col, const uint value)
{
    IoMessage ioMessage(string("rm"), value, "b");
    getBrainPort(row, col)->fwdToQueue(ioMessage);
}


vector<uint> *Brain::getValues(const uint row, const uint col)
{
    vector<uint> *values;
    auto cell = getPuzzleCell(row, col);
    if (cell) {
        values = cell->getPossibleValues();
    }
    return values;
}


void Brain::printSolution() {
    cout << "Solution" << endl;
    vector<std::string> solution;
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {

            if (auto cell = getPuzzleCell(row, col)) {
                uint value = cell->getSoleValue();
                if (value > 0) {
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


vector<puzzleState> Brain::getSolutionStruct() {
//    vector<uint> solution;
    vector<puzzleState> solution;
    uint rowcol = 0;
    for (puzzle_cell_ptr cell : puzzleCells_) {
        puzzleState ss{rowcol / 9, rowcol % 9, cell->getSoleValue(), *cell->getPossibleValues()};
//         solution.push_back(cell->getSoleValue());
        solution.push_back(ss);
        rowcol++;
    }
    return solution;
}


vector<uint> Brain::getSolution() {
    vector<uint> solution;
    uint rowcol = 0;
    for (puzzle_cell_ptr cell : puzzleCells_)
    {
         solution.push_back(cell->getSoleValue());
//        solution.push_back(ss);
        rowcol++;
    }
    return solution;
}


bool Brain::isPuzzleSolved() {
    bool solved = true;
    for (auto c : puzzleCells_) {
        if (c->getSoleValue() == 0) {
            solved = false;
            break;
        }
    }
    return solved;
}

vector<valStruct> Brain::getPossibleSolutions() {
    vector<valStruct> possibles;
    for (uint idx = 0; idx < 81; idx++) {
        puzzle_cell_ptr c = puzzleCells_.at(idx);
        if (c->getSoleValue() == 0) {
            vector<uint> *values = c->getPossibleValues();
            for (uint vidx = 1; vidx < 9; vidx++) {
                if (values->at(vidx) == 1) {
                    possibles.emplace_back(valStruct{idx / 9, idx % 9, vidx});
                }
            }
        }
    }
    return possibles;
}
