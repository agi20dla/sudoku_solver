//
// Created by jason on 4/17/18.
//

#include <iostream>
#include "GlobalCell.h"
#include "Puzzle.h"
#include "PuzzleCell.h"

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

    firstRun_ = true;
}


void Puzzle::setValue(const uint row, const uint col, const uint value) {
    // value may be 0 during initial puzzle setup when reading from
    // a file
    if (value == 0) {
        return;
    }
    getPuzzleCell(row, col)->setSoleValue(value);
    auto ioMessage = make_shared<IoMessage>(std::string("set"), value, std::string("b"), uuid_);
//    io_ptr mgtPort = getBrainPort(row, col);
//    mgtPort->fwdToQueue(ioMessage);
}


void Puzzle::setValue(const uint row, const uint col, const uint value, const vector<uint> possibles) {
    // value may be 0 during initial puzzle setup when reading from
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
//    getBrainPort(row, col)->fwdToQueue(ioMessage);
}


vector<uint> *Puzzle::getValues(const uint row, const uint col) {
    vector<uint> *values;
    auto cell = getPuzzleCell(row, col);
    values = cell->getPossibleValues();
    return values;
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


vector<CellState> Puzzle::getSolutionStates() {
    vector<CellState> solutionState;
    for (puzzle_cell_ptr cell : puzzleCells_) {
        CellState ss{cell->getSoleValue(), *cell->getPossibleValues()};
        solutionState.push_back(ss);
    }
    return solutionState;
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