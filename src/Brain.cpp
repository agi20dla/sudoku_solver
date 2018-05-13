//
// Created by jason on 6/5/15.
//

#include <string>
#include <vector>
#include <iostream>
#include <future>
//#include <boost/concept_check.hpp>
#include "Brain.h"
#include "Random.h"

using namespace std;
using namespace boost;

Brain::Brain() : uuid_(Random::getInstance().getNewUUID()) {
}

Brain::~Brain() {
//    solutionHashes_.clear();
    solutionPaths_.clear();
    brainPorts_.clear();
}


int Brain::solve(const vector<uint> vector) {

    unsigned maxThreads = std::thread::hardware_concurrency();
    if (maxThreads == 0) {
        maxThreads = 1;
    } else if (maxThreads > 1) {
        maxThreads -= 1;
    }
    cout << "Solving Sudoku Puzzle:" << endl;
    cout << "Can use " << maxThreads << " threads" << endl;
    printSolution();

    // Create first puzzle object and run it, set useQueue param to false
    Puzzle firstPuzzle(solutionPaths_);
    firstPuzzle.setValues(vector);
    bool solved = firstPuzzle.run();
    // Determine whether solved
    if (solved) {
        // great!  print solution and be done
    } else {

    }

    // if not, get SolutionPath and start brute forcing
    // possible SolutionPath's onto queue

    // put it on the queue
//    puzzleQueue_.push(puzzle);

    // spin up one thread and give it access to the queue
//    auto solved = puzzle.solve(puzzleQueue_);

    // Thread(s) begins taking puzzles off the queue.

    // Thread(s) put stalls on the queue

    // spin up additional threads for every 1000 stalls that are put on the queue

    // if a solutions is found, they will return that solution

    // if solved, then good to go.  print it and exit

    // if not solved, save off stalls to a queue

//    /*
//     *
//     * START THREADING HERE
//     *
//     */
//    if (firstRun_) {
//        // Run first time
//        if (!puzzle_.solve()) {
//            cerr << "Attempted to solve bad puzzle_" << endl;
//            return 1;
//        }
//        firstRun_ = false;
//    }
//
//    if (puzzle_.isPuzzleSolved()) {
//        cout << "Puzzle solved!" << endl;
//        printSolution();
//        return 0;
//    }
//
//    // store puzzle_ state and possible solutions
//    SolutionPath firstPath{getSolutionStates(), getPossibleSolutions()};
//    solutionHashes_.insert(firstPath);
//    solutionPaths_.push_back(firstPath);
//
//    // brute force a solution
//    while (!isPuzzleSolved() && solutionPaths_.size() > 0) {
//        SolutionPath path = solutionPaths_.front();
//        solutionPaths_.pop_front();
//        stalls_used_++;
//        vector<CellValue> possibles = path.possibles;
//        vector<CellState> puzzleStates = path.states;
//
//        // try all possible values on this state of the puzzle_
//        for (CellValue ss : possibles) {
//            setValue(ss.row, ss.col, ss.val);
//
////            // TODO: This is where we could set up additional processes to run the solutions down
////            // 1. set up (# cores - 1) threads with futures
////            // 2. iterate each thread for a true return, which happens when it finds a solution
////
////            auto result(std::future::async(std::launch::async, &Brain::run, this));
//////            future<bool> result(std::future::async(launch::async, &Brain::run, this));
//////            future<bool> result(async(launch::async, &Brain::run, this));
////            auto status = result.wait_for(std::chrono::milliseconds(0));
////            while (status != future_status::ready) {
////                status = result.wait_for(std::chrono::milliseconds(10));
////            }
////
////            bool b = result.get();
//            if (run()) {
//                if (isPuzzleSolved()) {
//                    break;
//                } else {
//                    stalls_++;
//                    // finished running with no exception, but no solution
//                    // so save this solution for the next iteration
//
//                    SolutionPath solPath{getSolutionStates(), getPossibleSolutions()};
//                    solutionPaths_.push_back(solPath);
//                }
//            } else {
//                failures_++;
//                // This happens when we've tried to run down a bad path
//                // reset all the cells for the next possible value
//                uint idx = 0;
//                for (auto c : puzzleCells_) {
//                    c->reset(puzzleStates.at(idx).soleValue, puzzleStates.at(idx).possibles);
//                    idx++;
//                }
//            }
//        }
//    }

    if (solved) {
        cout << "Puzzle solved!" << endl;
        printSolution();
    } else {
        cout << "Puzzle not solved!" << endl;
    }
    return 0;
}

boost::uuids::uuid Brain::getUUID() {
    return uuid_;
}

void Brain::reset() {
    brainPorts_.clear();
}


io_ptr Brain::getBrainPort(const uint row, const uint col) {
    io_ptr p = brainPorts_[row * 9 + col];
    return p;
}


bool operator< (const SolutionPath& lhs, const SolutionPath& rhs) {
    int lhsVal = 0, rhsVal = 0;
    for (auto lstate : lhs.states) {
        if (lstate.soleValue > 0) {
            lhsVal += 9;
        }
    }
    for (auto rstate : rhs.states) {
        if (rstate.soleValue > 0) {
            rhsVal += 9;
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
