//
// Created by jason on 5/23/15.
//

#include "PuzzleCell.h"

using namespace std;

PuzzleCell::PuzzleCell() :
        soleValue(0) {
    cellHub_->setCell(this);

//    io_ptr mgtPort = std::make_shared<IoPort>(mgtHub_, mgtMessages_, "m");
//    mgtHub_->addIoPort(mgtPort);

    io_ptr msgPort = std::make_shared<IoPort>(cellHub_, messages_, "m");
    cellHub_->addIoPort(msgPort);

//    mgtPort->connect(msgPort);
//    msgPort->connect(mgtPort);

//    mgtHub_->addPossibleValues(&possibleValues_);
}

PuzzleCell::PuzzleCell(const PuzzleCell &cell) {
    this->messages_ = cell.messages_;
//    this->mgtHub_ = cell.mgtHub_;
    this->soleValue = cell.soleValue;
}

//PuzzleCell::~PuzzleCell() {
//}

void PuzzleCell::run() {
    Cell::run();
//    mgtHub_->run();
}


//PuzzleCell &PuzzleCell::operator=(const PuzzleCell &other) {
//    if (this == &other) {
//        return *this;
//    }
//    this->cellHub_ = other.cellHub_;
//    this->mgtHub_ = other.mgtHub_;
//    return *this;
//}


vector<int_ptr> *PuzzleCell::getPossibleValues() {
    return &possibleValues_;
}
