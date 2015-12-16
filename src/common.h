//
// Created by jason on 11/18/15.
//

#ifndef SUDOKU_SOLVER_COMMON_H
#define SUDOKU_SOLVER_COMMON_H
using namespace std;

class PuzzleCell;

class IoPort;

class Hub;

class CellHub;

class Cell;

class IoMessage;

class GlobalCell;

typedef std::shared_ptr<PuzzleCell> puzzle_cell_ptr;
typedef std::shared_ptr<GlobalCell> global_cell_ptr;
typedef std::shared_ptr<IoPort> io_ptr;
typedef std::shared_ptr<Hub> hub_ptr;
typedef std::shared_ptr<CellHub> cell_hub_ptr;
typedef std::unordered_map<boost::uuids::uuid, uint, boost::hash<boost::uuids::uuid>> msg_map;
typedef std::shared_ptr<msg_map> msg_map_ptr;
#endif //SUDOKU_SOLVER_COMMON_H
