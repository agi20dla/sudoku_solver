//
// Created by jason on 11/18/15.
//

#ifndef SUDOKU_SOLVER_COMMON_H
#define SUDOKU_SOLVER_COMMON_H
using namespace std;

class Cell;

class IoPort;

class Hub;

class CellHub;

class MgtHub;

class IoMessage;

class BasicCell;

typedef std::shared_ptr<Cell> cell_ptr;
typedef std::shared_ptr<BasicCell> basic_cell_ptr;
typedef std::shared_ptr<IoPort> io_ptr;
typedef std::shared_ptr<Hub> hub_ptr;
typedef std::shared_ptr<CellHub> cell_hub_ptr;
typedef std::shared_ptr<MgtHub> mgt_hub_ptr;
typedef std::shared_ptr<IoMessage> msg_ptr;
#endif //SUDOKU_SOLVER_COMMON_H
