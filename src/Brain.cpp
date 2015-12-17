//
// Created by jason on 6/5/15.
//

#include <string>
#include<vector>
#include <iostream>
#include "Brain.h"
#include "GlobalCell.h"
#include "Random.h"

using namespace std;
using namespace boost;

Brain::Brain() : uuid_(Random::getInstance().getNewUUID()) { }

Brain::~Brain() {
    /*
     std::vector<puzzle_cell_ptr> puzzleCells_;
    std::vector<global_cell_ptr> globalCells_;
    std::vector<io_ptr> brainPorts_;

    // holds previous solutions when we get stuck
    std::vector<SolutionPath> solutionPaths_;
    std::unordered_set<SolutionPath, StateHasher> solutionHashes_;
     */
    solutionHashes_.clear();
    solutionPaths_.clear();
    brainPorts_.clear();
    globalCells_.clear();
    puzzleCells_.clear();
}

boost::uuids::uuid Brain::getUUID() {
    return uuid_;
}

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
        c->init();
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
                cell1->connect(cell2, std::string("h"));
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
                cell1->connect(cell2, std::string("v"));
            }
        }
    }
}


void Brain::createGlobalCells() {
    for (uint idx = 0; idx < 9; idx++) {
        global_cell_ptr g(make_shared<GlobalCell>());
        globalCells_.push_back(g);
    }
}


void Brain::connectGlobals() {
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            global_cell_ptr global = getGlobalCell(row/3, col/3);
            global->connect(getPuzzleCell(row, col), std::string("g"));
        }
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
    global_cell_ptr g = globalCells_.at(row * 3 + col);
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
        // Run first time
        if (!run(debug)) {
            cerr << "Attempeted to solve bad puzzle" << endl;
            return 1;
        }
        firstRun_ = false;
    }

    if (isPuzzleSolved()) {
        cout << "Puzzle solved!" << endl;
        printSolution();
        return 0;
    }

    // store puzzle state and possible solutions
    SolutionPath firstPath{getSolutionStruct(), getPossibleSolutions()};
    solutionHashes_.insert(firstPath);
    solutionPaths_.push_back(firstPath);

    // brute force a solution
    while (!isPuzzleSolved() && solutionPaths_.size() > 0) {
        SolutionPath path = solutionPaths_.back();
        solutionPaths_.pop_back();
        vector<CellValue> possibles = path.possibles;
        vector<PuzzleState> puzzleStates = path.states;

        for (CellValue ss : possibles) {
            setValue(ss.row, ss.col, ss.val);
            if (run(debug)) {
                if (isPuzzleSolved()) {
                    break;
                } else {
                    // finished running with no exception, but no solution
                    // so save this solution for the next iteration
                    SolutionPath solPath{getSolutionStruct(), getPossibleSolutions()};
                    if (solutionHashes_.find(solPath) == solutionHashes_.end()) {
                        auto it = solutionPaths_.begin();
                        for (; it != solutionPaths_.end(); it++) {
                            if ((*it) > solPath) {
                                solutionPaths_.insert(it, solPath);
                                break;
                            }
                        }
                        if (it == solutionPaths_.end()) {
                            solutionPaths_.push_back(solPath);
                        }

//                        solutionPaths_.push_back(solPath);
                    } else {
                        cout << "*************************** ";
                        cout << "Already tried this solution ";
                        cout << "*************************** " << endl;
                    }
                }
            } else {
                // This happens when we've tried to run down a bad path
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


bool Brain::run(bool debug) {
    bool firstRun = true;
    bool msgsRemaining = true;

    while (firstRun || msgsRemaining) {
        firstRun = false;
        numRuns_++;
        if (numRuns_ % 500 == 0 || debug) {
            cout << endl;
            cout << "Run number: " << numRuns_ << endl;
            printSolution();
        }
        // run the globals
        for (auto g : globalCells_) {
            g->run();
        }

        // run the cells
        for (auto c : puzzleCells_) {
            if (!c->run()) {
                return false;
            }
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

    return true;
}


void Brain::setValue(const uint row, const uint col, const uint value)
{
    // value may be 0 during initial puzzle setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    getPuzzleCell(row, col)->setSoleValue(value);
    auto ioMessage = make_shared<IoMessage>(std::string("set"), value, std::string("b"), uuid_);
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


void Brain::setValues(const vector<PuzzleState> solution) {
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
    auto ioMessage = std::make_shared<IoMessage>(string("rm"), value, std::string("b"), uuid_);
    getBrainPort(row, col)->fwdToQueue(ioMessage);
}


vector<uint> *Brain::getValues(const uint row, const uint col)
{
    vector<uint> *values;
    auto cell = getPuzzleCell(row, col);
    values = cell->getPossibleValues();
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


vector<PuzzleState> Brain::getSolutionStruct() {
    vector<PuzzleState> solution;
    for (puzzle_cell_ptr cell : puzzleCells_) {
        PuzzleState ss{cell->getSoleValue(), *cell->getPossibleValues()};
        solution.push_back(ss);
    }
    return solution;
}


vector<uint> Brain::getSolution() {
    vector<uint> solution;
    uint rowcol = 0;
    for (puzzle_cell_ptr cell : puzzleCells_)
    {
         solution.push_back(cell->getSoleValue());
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

vector<CellValue> Brain::getPossibleSolutions() {
    vector<CellValue> possibles;
    for (uint idx = 0; idx < 81; idx++) {
        puzzle_cell_ptr c = puzzleCells_.at(idx);
        if (c->getSoleValue() == 0) {
            vector<uint> *values = c->getPossibleValues();
            for (uint vidx = 1; vidx < 9; vidx++) {
                if (values->at(vidx) == 1) {
                    possibles.emplace_back(CellValue{idx / 9, idx % 9, vidx});
                }
            }
        }
    }
    return possibles;
}

bool operator< (const SolutionPath& lhs, const SolutionPath& rhs) {
    int lhsVal = 0, rhsVal = 0;
    for (auto lstate : lhs.states) {
        if (lstate.soleValue == 0) {
            lhsVal++;
        }
        for (auto p : lstate.possibles) {
            if (p == 0) {
                lhsVal++;
            } else {
                lhsVal--;
            }
        }
    }
    for (auto rstate : rhs.states) {
        if (rstate.soleValue == 0) {
            rhsVal++;
        }
        for (auto p : rstate.possibles) {
            if (p == 0) {
                rhsVal++;
            } else {
                rhsVal--;
            }
        }
    }
    return lhsVal < rhsVal;
}

bool operator> (const SolutionPath& lhs, const SolutionPath& rhs) {
    return rhs < lhs;
}

bool operator<= (const SolutionPath& lhs, const SolutionPath& rhs) {
    return !(lhs > rhs);
}

bool operator>= (const SolutionPath& lhs, const SolutionPath& rhs) {
    return !(lhs < rhs);
}
