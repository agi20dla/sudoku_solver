//
// Created by jason on 11/20/15.
//

#include "GlobalCell.h"
//#include "PuzzleCell.h"

using namespace std;

GlobalCell::GlobalCell() {
    cellHub_->setCell(this);
}

GlobalCell::GlobalCell(const GlobalCell &other) { }


GlobalCell &GlobalCell::operator=(const GlobalCell &other) {
    if (this == &other) {
        return *this;
    }
    this->cellHub_ = other.cellHub_;
    return *this;
}
