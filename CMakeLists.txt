# http://www.cmake.org/pipermail/cmake/2012-January/048419.html
set(CMAKE_GENERATOR "Ninja")

message("Generator is set to ${CMAKE_GENERATOR}")
cmake_minimum_required(VERSION 3.2)
project(sudoku_solver)

set(GMOCK_HOME /home/jason/src/gmock-1.7.0)
set(BOOST_ROOT /home/jason/src/boost_1_58_0)

include_directories(${GMOCK_HOME}/include ${GMOCK_HOME}/gtest/include ${BOOST_ROOT} $ENV{CPPUTEST_HOME}/include)

link_directories(${GMOCK_HOME}/mybuild ${GMOCK_HOME}/mybuild/gtest ${BOOST_ROOT}/libs $ENV{CPPUTEST_HOME}/lib)

find_package(Boost REQUIRED system thread exception)

find_file(Resources REQUIRED simple.grid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(SOURCE_FILES
        src/IoPort.h src/IoPort.cpp
        src/ConcurrentQueue.h
        src/Exceptions.h
        src/IoMessage.h src/IoMessage.cpp
        src/Hub.cpp src/Hub.h
        src/MgtHub.cpp src/MgtHub.h
        src/CellHub.cpp src/CellHub.h
        src/Cell.cpp src/Cell.h
        src/Brain.cpp src/Brain.h
        src/common.h)

set(TEST_FILES
        src/main.cpp
        test/ConcurrentQueueTest.cpp
        test/IoPortTest.cpp
        test/IOMessageTest.cpp
        test/HubTest.cpp
        test/IntegrationTests.cpp
        test/CellTest.cpp
        test/BrainTest.cpp

        )

add_executable(sudoku_solver ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sudoku_solver ${Boost_LIBRARIES})
target_link_libraries(sudoku_solver gmock)
target_link_libraries(sudoku_solver gtest)
target_link_libraries(sudoku_solver pthread)
target_link_libraries(sudoku_solver CppUTest)


