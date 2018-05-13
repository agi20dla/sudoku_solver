set(CMAKE_GENERATOR "Ninja")
message("Generator is set to ${CMAKE_GENERATOR}")

cmake_minimum_required(VERSION 3.10)
project(sudoku_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

set(GMOCK_HOME /home/jason/src/googletest/googlemock)
set(GTEST_HOME /home/jason/src/googletest/googletest)
set(BOOST_ROOT /home/jason/src/boost)
set(BOOST_INCLUDEDIR /home/jason/src/boost)

include_directories(${GMOCK_HOME}/include ${GTEST_HOME}/include ${BOOST_INCLUDEDIR} $ENV{CPPUTEST_HOME}/include)
link_directories(${BOOST_ROOT}/stage/lib)
find_package(Boost REQUIRED system thread exception)

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

set(SOURCE_FILES
        src/main.cpp
        src/IoPort.h src/IoPort.cpp
        #        src/ConcurrentQueue.h
        #        src/Exceptions.h
        #        src/IoMessage.h src/IoMessage.cpp
        #        src/CellHub.h src/CellHub.cpp
        #        src/PuzzleCell.h src/PuzzleCell.cpp
        #        src/Brain.h src/Brain.cpp
        src/common.h
        #        src/GlobalCell.h src/GlobalCell.cpp
        #        src/Data.h src/Data.cpp
        #        src/Cell.cpp src/Cell.h
        #        src/SolutionMatrix.cpp src/SolutionMatrix.h
        #        src/Puzzle.cpp src/Puzzle.h
        )

set(TEST_FILES
        #        test/ConcurrentQueueTest.cpp
        test/IoPortTest.cpp
        #        test/HubTest.cpp
        #        test/IntegrationTests.cpp
        #        test/CellTest.cpp
        #        test/BrainTest.cpp
        #        test/DataTest.cpp
        #        test/SolutionTest.cpp
        #        src/Hub.cpp src/Hub.h
        #        test/SpeedTest.cpp
        #        test/RandomTest.cpp
        #        test/PuzzleTest.cpp src/concurrentqueue.h
        )

add_executable(sudoku_solver ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(sudoku_solver ${Boost_LIBRARIES})
target_link_libraries(sudoku_solver gmock)
target_link_libraries(sudoku_solver gtest)
target_link_libraries(sudoku_solver pthread)

set(CMAKE_BUILD_TYPE Release)
