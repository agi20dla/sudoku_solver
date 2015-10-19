# sudoko_solver
A highly over-engineered C++ program to solver sudoku puzzles.

It's purpose is as a learning tool for the author, me, Jason C. Sherman.  I taught myself C++ over 2 decades ago
using Microsoft Visual C++ and wrote a fairly good sized app with it.  But, I wanted to learn C++11/14, boost,
TDD in C++, concurrency, multithreading, etc.  I came up with the genious idea of a program that solves sudoku
problems, modled after my impression of how neurons are connected together (I am *NOT* a neuroscientist).

Each cell is completely independent of the other cells.  They communicate by passing messages out of, or into, IO ports.
As far as each cell knows, it's just getting or sending a message.  It really has no idea where the message is going,
or where it came from.  The cell with take some action based on the message content and forward the message along,
if needed.
