#sudoko_solver
##A highly over-engineered C++ program to solver sudoku puzzles.

It's purpose is as a learning tool for the author, me, Jason C. Sherman.  I taught myself C++ over 2 decades ago
using Microsoft Visual C++ and wrote a fairly good sized app with it.  But, I wanted to learn C++11/14, boost,
TDD in C++, concurrency, multithreading, etc.  I came up with the genious idea of a program that solves sudoku
problems, modeled after my impression of how neurons are connected together (I am *NOT* a neuroscientist and there
are no weights attached to any cell's input or output).

Hopefully the following discussion of the problem, program, and solution won't get too convoluted.

## Sudoku
If you don't know what a Sudoku puzzle is, check out the wikipedia article:

https://en.wikipedia.org/wiki/Sudoku

This program logically breaks up the puzzle into three parts:

1. The overall puzzle, accessible from the Brain, which can also access each individual Cell and Group cell through
special management connections called brain (b) ports (explained later).

2. The 9 groups of 9 cells, which are connected internally through specialized Group cells, via group (g) ports, that
allow communication between each cell in a group, but not outside of it.

3. The 81 individual cells, which are connected via horizontal (h) and vertical (v) ports that allow communication
between cells on a horizontal or vertical path in the puzzle

Each cell contains a Cell Hub and a Management Hub, which are connected to each other via an internal management (m)
port.  These hubs are concurrent queues that are used to buffer the messages being passed around.

The Brain sets up the problem and runs each cell, in-turn (or several at a time in a multithreaded, concurrent fashion,
but I'm not there yet).  Each cell is completely independent of the other cells.
They communicate by passing messages through the IO Ports mentioned earlier.
As far as each cell knows, it's just getting or sending a message.  It only knows that a message came from a certain
port and that it needs to go out of a certain port.  The management portion of the cell will take some action based on
the message content, if needed.

## For Developers, use these conventions

###Branches should be named with the following prefix tokens.  e.g. feat/readfile
wip       Works in progress; stuff I know won't be finished soon
feat      Feature I'm adding or expanding
bug       Bug fix or experiment
junk      Throwaway branch created to experiment
