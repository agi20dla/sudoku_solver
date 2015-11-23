#include <iostream>
#include "gmock/gmock.h"
#include <execinfo.h>

using namespace std;


/**
 * Shamelessly stolen from:
 * http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
 *
 * TODO: To be taken out in final release, but staying here while building the program
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

    // TODO: take out the SIGSEGV handler in final code
    signal(SIGSEGV, handler);

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
