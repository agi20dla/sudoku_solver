#include <iostream>
#include "gmock/gmock.h"

using namespace std;

int main(int argc, char **argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}