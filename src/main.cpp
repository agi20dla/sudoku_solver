#include <iostream>
#include "gmock/gmock.h"
#include "Data.h"
#include "Brain.h"
#include <execinfo.h>
#include <thread>

//int readFile(char *string);

using namespace std;


/**
 * Shamelessly stolen from:
 * http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
 *
 * To be taken out in final release, but staying here while building the program
 */
void handler(int sig) {
    void *array[10];
    int size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char **argv) {
    signal(SIGSEGV, handler);

    if (argc == 3 || argc == 1) {
        cout << "Running internal tests";
        testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }
    else if (argc != 2) {
        cerr << "Usage: " << endl;
        cerr << "To solve a specific problem: " << argv[0] << " FILENAME" << endl;

        cerr << "or" << endl;
        cerr << "To run internal tests: " << argv[0] << endl;
        return 1;
    }

    string filename(argv[1]);
    Data data;
    data.init(filename);

    // Somebody get this man a BRAIN!
    Brain brain;
//    brain.reset();
//    brain.setValues(data.getDataAsVector());

    // Tell the brain to solve the provided puzzle_
    int result = brain.solve(data.getDataAsVector());

    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Waited " << elapsed.count() << " ms\n";

    return result;
}
