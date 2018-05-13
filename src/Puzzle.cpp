//
// Created by jason on 4/17/18.
//

#include <iostream>
#include "GlobalCell.h"
#include "Puzzle.h"
#include "PuzzleCell.h"
#include "Random.h"

Puzzle::Puzzle() : uuid_(Random::getInstance().getNewUUID()) {}

Puzzle::Puzzle(ConcurrentQueue<SolutionPath> solutionsPaths) {
    this->solutionPaths_ = solutionsPaths;
}

Puzzle::~Puzzle() {

    globalCells_.clear();

    while (puzzleCells_.size() > 0) {
        puzzle_cell_ptr c = puzzleCells_.back();
        puzzleCells_.pop_back();
        c.reset();
    }
    puzzleCells_.clear();
}

int Puzzle::solve(bool useQueue) {
    /*
     *
     * START THREADING HERE
     *
     */
    if (firstRun_) {
        // Run first time
        if (!run()) {
            cerr << "Attempted to solve bad puzzle_" << endl;
            return 1;
        }
        firstRun_ = false;
    }

    if (isPuzzleSolved()) {
        cout << "Puzzle solved!" << endl;
        printSolution();
        return 0;
    }

    // store puzzle_ state and possible solutions
    SolutionPath firstPath{getSolutionStates(), getPossibleSolutions()};
//    solutionHashes_.insert(firstPath);
    solutionPaths_.push_back(firstPath);

    // brute force a solution
    while (!isPuzzleSolved() && !solutionPaths_.empty()) {
        SolutionPath path = solutionPaths_.front();
        solutionPaths_.pop_front();
        stalls_used_++;
        vector<CellValue> possibles = path.possibles;
        vector<CellState> puzzleStates = path.states;

        // try all possible values on this state of the puzzle_
        for (CellValue ss : possibles) {
            setValue(ss.row, ss.col, ss.val);

//            // TODO: This is where we could set up additional processes to run the solutions down
//            // 1. set up (# cores - 1) threads with futures
//            // 2. iterate each thread for a true return, which happens when it finds a solution
//
//            auto result(std::future::async(std::launch::async, &Brain::run, this));
////            future<bool> result(std::future::async(launch::async, &Brain::run, this));
////            future<bool> result(async(launch::async, &Brain::run, this));
//            auto status = result.wait_for(std::chrono::milliseconds(0));
//            while (status != future_status::ready) {
//                status = result.wait_for(std::chrono::milliseconds(10));
//            }
//
//            bool b = result.get();
            if (run()) {
                if (isPuzzleSolved()) {
                    break;
                } else {
                    stalls_++;
                    // finished running with no exception, but no solution
                    // so save this solution for the next iteration

                    SolutionPath solPath{getSolutionStates(), getPossibleSolutions()};
                    solutionPaths_.push_back(solPath);
                }
            } else {
                failures_++;
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

    printSolution();
    return 0;
}


vector<CellState> Puzzle::getSolutionStates() {
    vector<CellState> solutionState;
    for (puzzle_cell_ptr cell : puzzleCells_) {
        CellState ss{cell->getSoleValue(), *cell->getPossibleValues()};
        solutionState.push_back(ss);
    }
    return solutionState;
}


vector<CellValue> Puzzle::getPossibleSolutions() {
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


bool Puzzle::run() {
    bool msgsRemaining = true;

    while (msgsRemaining) {
        numRuns_++;
//        if (numRuns_ % 500 == 0) {
//            cout << endl;
//            printSolution();
//        }
        // run the globals
        for (auto g : globalCells_) {
            g->run();
        }

        // run the cells
        for (auto c : puzzleCells_) {
            if (!c->run()) {
                // failed run, so bounce out
                return false;
            }
        }

        // check whether any messages are remaining to be processed
        msgsRemaining = false;
        for (auto pCell : puzzleCells_) {
            msgsRemaining = pCell->hubHasMessages();
            if (msgsRemaining) {
                break;
            }
        }
        if (!msgsRemaining) {
            for (auto gCell : globalCells_) {
                msgsRemaining = gCell->hubHasMessages();
                if (msgsRemaining) {
                    break;
                }
            }
        }
    }

    return true;
}
void Puzzle::createPuzzleCells() {
    for (uint idx = 0; idx < 81; idx++) {
        puzzle_cell_ptr c(make_shared<PuzzleCell>());
        c->init();
        puzzleCells_.push_back(c);
    }
}


void Puzzle::connectPuzzleRows() {
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


void Puzzle::connectPuzzleCols() {
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


void Puzzle::createGlobalCells() {
    for (uint idx = 0; idx < 9; idx++) {
        global_cell_ptr g(make_shared<GlobalCell>());
        globalCells_.push_back(g);
    }
}


void Puzzle::connectGlobals() {
    for (uint row = 0; row < 9; row++) {
        for (uint col = 0; col < 9; col++) {
            global_cell_ptr global = getGlobalCell(row / 3, col / 3);
            global->connect(getPuzzleCell(row, col), std::string("g"));
        }
    }
}


puzzle_cell_ptr Puzzle::getPuzzleCell(const uint row, const uint col) {
    puzzle_cell_ptr c = puzzleCells_.at(row * 9 + col);
    return c;
}

std::vector<puzzle_cell_ptr> Puzzle::getPuzzleCells() {
    return puzzleCells_;
}

global_cell_ptr Puzzle::getGlobalCell(const uint row, const uint col) {
    global_cell_ptr g = globalCells_.at(row * 3 + col);
    return g;
}

void Puzzle::reset() {
    puzzleCells_.clear();
    globalCells_.clear();

    createPuzzleCells();
    connectPuzzleRows();
    connectPuzzleCols();
    createGlobalCells();
    connectGlobals();

    connectPuzzleToPuzzleCells();
    firstRun_ = true;
}


void Puzzle::setValue(const uint row, const uint col, const uint value) {
    // value may be 0 during initial puzzle_ setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    getPuzzleCell(row, col)->setSoleValue(value);
    auto ioMessage = make_shared<IoMessage>(std::string("set"), value, std::string("b"), uuid_);
    io_ptr mgtPort = getPuzzlePort(row, col);
    mgtPort->fwdToQueue(ioMessage);
}


void Puzzle::setValue(const uint row, const uint col, const uint value, const vector<uint> possibles) {
    // value may be 0 during initial puzzle_ setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    auto c = getPuzzleCell(row, col);
    c->setSoleValue(value);
    c->setPossibles(possibles);
}


void Puzzle::setValues(const vector<CellState> solution) {
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

void Puzzle::setValues(const vector<uint> values) {
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


void Puzzle::removeValue(const uint row, const uint col, const uint value) {
    auto ioMessage = std::make_shared<IoMessage>(string("rm"), value, std::string("b"), uuid_);
    getPuzzlePort(row, col)->fwdToQueue(ioMessage);
}


vector<uint> *Puzzle::getValues(const uint row, const uint col) {
    vector<uint> *values;
    auto cell = getPuzzleCell(row, col);
    values = cell->getPossibleValues();
    return values;
}


vector<uint> Puzzle::getSolution() {
    vector<uint> solution;
    uint rowcol = 0;
    for (puzzle_cell_ptr cell : puzzleCells_) {
        solution.push_back(cell->getSoleValue());
        rowcol++;
    }
    return solution;
}


bool Puzzle::isPuzzleSolved() {
    bool solved = true;
    for (auto c : puzzleCells_) {
        if (c->getSoleValue() == 0) {
            solved = false;
            break;
        }
    }
    return solved;
}

boost::uuids::uuid Puzzle::getUUID() {
    return uuid_;
}

void Puzzle::connectPuzzleToPuzzleCells() {
    for (auto c : getPuzzleCells()) {
        puzzlePorts_.push_back(c->connect("b"));
    }

}

io_ptr Puzzle::getPuzzlePort(const uint row, const uint col) {
    io_ptr p = puzzlePorts_[row * 9 + col];
    return p;
}

void Puzzle::printSolution() {

    cout << "Run number: " << numRuns_ << endl;
    cout << "Total number of stalls: " << stalls_ << endl;
    cout << "Number of stalls left: " << solutionPaths_.size() << endl;
    cout << "Number of stalls used: " << stalls_used_ << endl;
    cout << "Number of failures: " << failures_ << endl;
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

