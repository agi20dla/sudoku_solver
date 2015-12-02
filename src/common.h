//
// Created by jason on 11/18/15.
//

#ifndef SUDOKU_SOLVER_COMMON_H
#define SUDOKU_SOLVER_COMMON_H
using namespace std;

class PuzzleCell;

class IoPort;

class CellHub;

class Cell;

class IoMessage;

class GlobalCell;

//class Cell;
//
//typedef std::shared_ptr<Cell> cell_ptr;
typedef std::shared_ptr<Cell> cell_ptr;
typedef std::shared_ptr<PuzzleCell> puzzle_cell_ptr;
typedef std::shared_ptr<GlobalCell> global_cell_ptr;
typedef std::shared_ptr<IoPort> io_ptr;
typedef std::shared_ptr<CellHub> cell_hub_ptr;
typedef std::shared_ptr<IoMessage> msg_ptr;
typedef std::shared_ptr<int> int_ptr;
#endif //SUDOKU_SOLVER_COMMON_H
