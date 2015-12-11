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

configure_file(test/resources/02_easy.pzl ${CMAKE_CURRENT_BINARY_DIR}/02_easy.pzl COPYONLY)
configure_file(test/resources/02_answer.pzl ${CMAKE_CURRENT_BINARY_DIR}/02_answer.pzl COPYONLY)
configure_file(test/resources/30_easy.pzl ${CMAKE_CURRENT_BINARY_DIR}/30_easy.pzl COPYONLY)
configure_file(test/resources/30_answer.pzl ${CMAKE_CURRENT_BINARY_DIR}/30_answer.pzl COPYONLY)
configure_file(test/resources/32_medium.pzl ${CMAKE_CURRENT_BINARY_DIR}/32_medium.pzl COPYONLY)
configure_file(test/resources/32_answer.pzl ${CMAKE_CURRENT_BINARY_DIR}/32_answer.pzl COPYONLY)
configure_file(test/resources/67_hard.pzl ${CMAKE_CURRENT_BINARY_DIR}/67_hard.pzl COPYONLY)
configure_file(test/resources/67_answer.pzl ${CMAKE_CURRENT_BINARY_DIR}/67_answer.pzl COPYONLY)
configure_file(test/resources/100_diabolical.pzl ${CMAKE_CURRENT_BINARY_DIR}/100_diabolical.pzl COPYONLY)
configure_file(test/resources/100_answer.pzl ${CMAKE_CURRENT_BINARY_DIR}/100_answer.pzl COPYONLY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

set(SOURCE_FILES
        src/main.cpp
        src/IoPort.h src/IoPort.cpp
        src/ConcurrentQueue.h
        src/Exceptions.h
        src/IoMessage.h src/IoMessage.cpp
        src/CellHub.h src/CellHub.cpp
        src/PuzzleCell.h src/PuzzleCell.cpp
        src/Brain.h src/Brain.cpp
        src/common.h
        src/GlobalCell.h src/GlobalCell.cpp
        src/Data.h src/Data.cpp src/Cell.cpp src/Cell.h src/Random.h)

set(TEST_FILES
        test/ConcurrentQueueTest.cpp
        test/IoPortTest.cpp
        test/HubTest.cpp
        test/IntegrationTests.cpp
        test/CellTest.cpp
        test/BrainTest.cpp
        test/DataTest.cpp
        test/SolutionTest.cpp src/Hub.cpp src/Hub.h test/SpeedTest.cpp)

add_executable(sudoku_solver ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sudoku_solver ${Boost_LIBRARIES})
target_link_libraries(sudoku_solver gmock)
target_link_libraries(sudoku_solver gtest)
target_link_libraries(sudoku_solver pthread)
target_link_libraries(sudoku_solver CppUTest)
