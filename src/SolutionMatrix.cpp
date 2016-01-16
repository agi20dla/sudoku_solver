//
// Created by jason on 12/17/15.
//

#include <sys/types.h>
#include <iostream>
#include "SolutionMatrix.h"

using namespace std;
SolutionMatrix::SolutionMatrix() {
    for (int idx = 0; idx < 81; idx++) {
        vector<long> defaultVals{0,0,0,0,0,0,0,0,0,0};
        guess_.push_back(defaultVals);
    }
}


void SolutionMatrix::addNegStates(vector<CellState> states ) {
    for (int idx = 0; idx < 81; idx++) {
        vector<long> cellGuess = guess_.at(idx);
        uint soleValue = states.at(idx).soleValue;
        if (soleValue > 0) {
            long &guessVal = cellGuess.at(soleValue);
            guessVal -= 2;
        }
    }
}

void SolutionMatrix::addPosStates(vector<CellState> states ) {
    for (int idx = 0; idx < 81; idx++) {
//        ++(guess_.at(idx).at(states.at(idx).soleValue));

        vector<long> &cellGuess = guess_.at(idx);
        uint soleValue = states.at(idx).soleValue;
        if (soleValue > 0) {
            long &guessVal = cellGuess.at(soleValue);
            ++guessVal;
        }
    }
}

void SolutionMatrix::printGuess() {
    cout << "Current Guess" << endl;
    vector<std::string> solution;
    for (int idx = 0; idx < 81; idx++) {
        uint guess = getGuess(guess_.at(idx));
        if (guess == 0) {
            solution.push_back("*");
        } else {
            solution.push_back(to_string(guess));
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

uint SolutionMatrix::getGuess(vector<long> guesses) {
    uint guess = 0;
    long max = guesses.at(1);
    for (uint idx = 2; idx < 10; idx++) {
        if (guesses.at(idx) > max) {
            max = guesses.at(idx);
            guess = idx;
        }
    }
    return guess;
}
